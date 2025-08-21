#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ASCII_UPPERBOUND 122
#define ASCII_LOWERBOUND 32

#define DELAY 5000 // Microseconds

#define clear() printf("\033[H\033[J")
#define DIE(fmt, d) \
	fprintf(stderr, fmt, d); \
	free(buffer); \
	return 1;

char* 			buffer;
unsigned int 	buffer_position = 0;
unsigned int 	buffer_size 	= 0;
char 			cascii 			= ASCII_LOWERBOUND;
unsigned int 	ascii_misses 	= 0;

bool opt_clear = false; /* Clears screen for printing */

static void
usage(const char* progname)
{
	printf("usage: %s [-c] WORDS...\n", progname);
	printf("\t-c\t clears the screen every write to stdout\n");
}

static char
cycle_until_match(const char match)
{
	while (ascii_misses < 3) {
		if ( opt_clear ) { clear(); }
		printf("\r%s", buffer);
		printf("%c", cascii);
		fflush(stdout);

		if ( match == cascii ) {
			ascii_misses = 0;
			return cascii;
		} else if ( ++cascii > ASCII_UPPERBOUND ) {
			cascii = ASCII_LOWERBOUND;
			ascii_misses++;
		}

		usleep(DELAY);
	}
	
	return 0;
}

int
main(int argc, char** argv)
{
	char match;
	int args_start_index = 1;

	if ( argc == 1 ) {
		usage(argv[0]);
		return 0;
	}

	if ( argc > 1 && argv[1][0] == '-' ) {
		args_start_index = 2;
		for ( int i = 1; i < strlen(argv[1]); i++ ) {
			char opt = argv[1][i];
			if ( opt == 'c' ) {
				opt_clear = true;			
			} else if ( opt == 'h' ) {
				usage(argv[0]);
				return 0;
			}
		}
	}

	for (int i = args_start_index; i < argc; i++ ) {
		/* + 1 for space character */
		buffer_size += strlen(argv[i]) + 1;
	}

	buffer = calloc(++buffer_size, sizeof(char));

	for ( int i = args_start_index; i < argc; i++ ) {
		for ( int j = 0; j < strlen(argv[i]); j++ ) {
			match = cycle_until_match(argv[i][j]);

			if ( !match ) {
				DIE("\nFailed to match character '%c'\n", argv[i][j]);
			}

			buffer[buffer_position++] = match;
		}

		buffer[buffer_position++] = ' ';
	}

	printf("\n");
	free(buffer);
	return 0;
}

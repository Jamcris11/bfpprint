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
unsigned int 	buffer_size 	= 1; // 1 for NULL byte
char 			cascii 			= ASCII_LOWERBOUND;
unsigned int 	ascii_misses 	= 0;

static char
cycle_until_match(const char match)
{
	while (ascii_misses < 3) {
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

	for (int i = 1; i < argc; i++ ) {
		/* + 1 for space character */
		buffer_size += strlen(argv[i]) + 1;
	}

	buffer = calloc(buffer_size, sizeof(char));

	for ( int i = 1; i < argc; i++ ) {
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

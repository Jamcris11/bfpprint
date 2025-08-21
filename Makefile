BIN=bfpp

.PHONY: build
build:
	gcc -o $(BIN) main.c

install:
	cp $(BIN) /usr/bin/$(BIN)

uninstall:
	rm /usr/bin/$(BIN)

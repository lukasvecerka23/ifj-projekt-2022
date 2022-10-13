PROJECT=lexeme
CC=gcc
PROG=$(PROJECT)-test
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon

test-run: $(PROG)

$(PROJECT)-test: $(PROJECT).c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c
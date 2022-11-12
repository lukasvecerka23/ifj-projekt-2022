PROJECT=lexeme
CC=gcc
PROG=$(PROJECT)-test
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
EXEC = main_prg
OBJ = main_prg.o lexeme.o error.o parser.o

# test-run: $(PROG)
run: main_prg
	./main_prg

# $(PROJECT)-test: $(PROJECT).c
# 	$(CC) $(CFLAGS) -o $@ $(PROJECT).c
main_prg: main_prg.o lexeme.o error.o parser.o
	$(CC) $(CFLAGS) $(OBJ) -o main_prg

main_prg.o: main_prg.c
	$(CC) $(CFLAGS) -c main_prg.c -o main_prg.o

lexeme.o: lexeme.c lexeme.h
	$(CC) $(CFLAGS) -c lexeme.c -o lexeme.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

clean: 
	rm $(EXEC) *.o

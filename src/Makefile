PROJECT=lexeme
CC=gcc
PROG=$(PROJECT)-test
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
EXEC = main_prg
OBJ = main_prg.o lexeme.o error.o parser.o hashtable.o

.PHONY: main_prg

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main_prg

run: main_prg
	./main_prg

main_prg.o: main_prg.c
	$(CC) $(CFLAGS) -c main_prg.c -o main_prg.o

lexeme.o: lexeme.c lexeme.h
	$(CC) $(CFLAGS) -c lexeme.c -o lexeme.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

clean: 
	rm $(EXEC) *.o

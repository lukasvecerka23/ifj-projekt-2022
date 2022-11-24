PROJECT=ifj_2022
CC=gcc
PROG=$(PROJECT)-test
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
EXEC = ifj22_compiler
OBJ = ifj22_compiler.o scanner.o error.o parser.o symtable.o code_gen.o

.PHONY: ifj22_compiler

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

run: ifj22_compiler
	./ifj22_compiler

ifj22_compiler.o: ifj22_compiler.c
	$(CC) $(CFLAGS) -c ifj22_compiler.c -o ifj22_compiler.o

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

code_gen.o: code_gen.c code_gen.h
	$(CC) $(CFLAGS) -c code_gen.c -o code_gen.o

clean: 
	rm $(EXEC) *.o

CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm
FILES=hashtable.c hashtable_test.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

run: test
	@./test
	
clean:
	rm -f test

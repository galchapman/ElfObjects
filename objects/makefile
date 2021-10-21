CC=gcc
CFLAGS=-std=c17 -g -Wall

test: build/test
	./$^

build: build/test build/main


build/test: test.c build/out.o
	$(CC) $(CFLAGS) $^ -o $@

build/main: main.c
	$(CC) $(CFLAGS) $^ -o $@

build/out.o: build/main
	./build/main

clean:
	rm build/main
	rm build/loader.o
CFLAGS = -g -Wall -fpic -lm -std=c99

default: all

all: ftserver.o
	gcc main.c ftserver.o -o ftserver $(CFLAGS)
	
clean:
	rm -f ftserver.o ftserver

ftserver.o: ftserver.c
	gcc -c ftserver.c -o ftserver.o $(CFLAGS)

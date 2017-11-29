CFLAGS = -Wall -fpic -coverage -lm -std=c99 -ftest-coverage -fprofile-arcs

all:
	gcc -o testme -g testme.c $(CFLAGS)

clean:
	rm -f testme testmeresults.out
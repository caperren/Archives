all: clean build
	
clean:
	rm -f *.o
	rm -f concurrency1
	
build:
	gcc main.c mt19937ar.h -o concurrency1 -std=c99 -pthread
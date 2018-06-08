all: clean build
	
clean:
	rm -f *.o
	rm -f concurrency2
	
build:
	gcc main.c mt19937ar.h -o concurrency2 -std=c99 -lpthread -lrt
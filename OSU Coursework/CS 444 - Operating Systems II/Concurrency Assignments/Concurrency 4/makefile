all: clean build
	
clean:
	rm -f *.o
	rm -f concurrency4
	
build:
	gcc main.c mt19937ar.h -o concurrency4 -std=c99 -lpthread -lrt

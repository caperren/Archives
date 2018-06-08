all: clean build
	
clean:
	rm -f *.o
	rm -f concurrency3_problem1 concurrency3_problem2
	
build:
	gcc problem1.c mt19937ar.h -o concurrency3_problem1 -std=c99 -lpthread -lrt
	gcc problem2.c mt19937ar.h -o concurrency3_problem2 -std=c99 -lpthread -lrt

/* 
 * File:   main.c
 * Author: caperren
 *
 * Created on April 13, 2018, 7:27 PM
 */

/*
 * Important implementation notes...
 * Consumer prints out number in struct on consumption
 * Timeout is how long before consumer is done with item (2-7 seconds)
 * Producer waits 3-7 seconds
 * Max buffer size is 32
 * Consumer/s and producer need lock on buffer when doing things
 * If nothing in queue, consumer blocks until there is
 */

#include "mt19937ar.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

// Program defines you might want to change
#define MAX_BUFFER_SIZE 32

#define MIN_RANDOM_PRODUCER 3
#define MAX_RANDOM_PRODUCER 7

#define MIN_RANDOM_CONSUMER 2
#define MAX_RANDOM_CONSUMER 9

//RDRAND value to check for
#define RDRAND_FLAG (1 << 30)

// Data structure for the buffer elements
struct two_values {
    uint8_t number;
    uint8_t work_timeout;
};

//Storage containers and mutex for access
struct two_values storage_buffer[MAX_BUFFER_SIZE];
uint16_t tasks_in_buffer = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Get random number based on whether rdrand is supported.
uint8_t get_random_number(uint8_t min, uint8_t max) {
    uint8_t rand_val;
    unsigned long seed;

    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;

    eax = 0x01;

    // ASM for checking if RDRAND supported
    __asm__ __volatile__("cpuid"
            : "=a" (eax),
            "=b" (ebx),
            "=c" (ecx),
            "=d" (edx)
            : "a" (1),
            "c" (0)
            );

    // Seed random with /dev/random
    int random = open("/dev/random", O_RDONLY);
    read(random, &seed, sizeof (seed));

    unsigned int random_value;

    if ((ecx & RDRAND_FLAG) == RDRAND_FLAG) {
	char return_code = 0;
	char count = 0;

        // ASM for RDRAND
	while(return_code != 1 && count < 10){
		__asm__ volatile(
		        "rdrand %0 ; setc %1"
		        : "=r" (random_value), "=qm" (return_code)
		);

		count++;
	}
    } else {
        // Twister if RDRAND not supported
        init_genrand(seed);
	random_value = genrand_int32();
    }

    return (random_value % (max + 1 - min)) + min;
}

void *consume_buffer_items(){
    uint8_t number;
    uint8_t work_timeout;
    
    while(1){
        // This waits for a task to exist, then makes a temporary copy
        // of the data in the last item in the queue.
        // It then removes the item, while mutex locking during all of this
        while(tasks_in_buffer == 0);
        pthread_mutex_lock(&mutex);
        
        number = storage_buffer[tasks_in_buffer - 1].number;
        work_timeout = storage_buffer[tasks_in_buffer - 1].work_timeout;
        storage_buffer[tasks_in_buffer - 1].number = 0;
        storage_buffer[tasks_in_buffer - 1].work_timeout = 0;
        tasks_in_buffer--;
        
        pthread_mutex_unlock(&mutex);
        
        printf("Consumer found number: %u. Now waiting %u seconds.\n", number, work_timeout);
        fflush(stdout);
        
        sleep(work_timeout);   
    }
    
}

int main(int argc, char **argv) {  
    pthread_t consumer_thread;
    
    // This makes the consumer thread
    if(pthread_create(&consumer_thread, NULL, consume_buffer_items, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    
    //Zero out array
    for(uint16_t i = 0 ; i < MAX_BUFFER_SIZE ; i++){
        storage_buffer[i].number = 0;
        storage_buffer[i].work_timeout = 0;
    }
    
    uint8_t number;
    uint8_t work_timeout;
    uint8_t producer_sleep;
    
    while(1){
        // This generates values for a new buffer element, waits until theres
        // space in the buffer, then adds and item to the buffer
        number = get_random_number(0, UINT8_MAX);
        work_timeout = get_random_number(MIN_RANDOM_CONSUMER, MAX_RANDOM_CONSUMER);
        producer_sleep = get_random_number(MIN_RANDOM_PRODUCER, MAX_RANDOM_PRODUCER);
        
        while(tasks_in_buffer == MAX_BUFFER_SIZE);
        
        pthread_mutex_lock(&mutex);
        storage_buffer[tasks_in_buffer].number = number;
        storage_buffer[tasks_in_buffer].work_timeout = work_timeout;
        tasks_in_buffer++;
        pthread_mutex_unlock(&mutex);
        
        printf("Producer adding number: %u. Consumers's delay will be %u seconds. Producer waiting %u seconds.\n", number, work_timeout, producer_sleep);
        fflush(stdout);
        
        sleep(producer_sleep);
    }

    return 0;
}


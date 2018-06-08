/* 
 * File:   main.c
 * Author: caperren
 *
 * Created on April 27th, 2018
 */

#include "mt19937ar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/param.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Program defines you might want to change
#define NUM_PHILOSOPHERS 5

#define MIN_RANDOM_EAT 2
#define MAX_RANDOM_EAT 9

#define MIN_RANDOM_THINK 1
#define MAX_RANDOM_THINK 20

uint8_t* philosopher_names[] = {
    "Plato",
    "Aristotle",
    "Socrates",
    "Confucius",
    "Voltaire"    
};

//RDRAND value to check for
#define RDRAND_FLAG (1 << 30)

sem_t fork_semaphores[NUM_PHILOSOPHERS];
uint32_t philosopher_eat_counts[NUM_PHILOSOPHERS];

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

void think(uint8_t *name){
    uint8_t random = get_random_number(MIN_RANDOM_THINK, MAX_RANDOM_THINK);
    
    printf("%s thinking for %u seconds.\n", name, random);
    fflush(stdout);
    
    sleep(random);
}

void eat(uint8_t *name){
    uint8_t random = get_random_number(MIN_RANDOM_EAT, MAX_RANDOM_EAT);
    
    printf("%s eating for %u seconds.\n", name, random);
    fflush(stdout);
    
    sleep(random);
}

void *philosopher(void *number){
    // Store values that will be useful as the thread runs
    uint16_t philosophers_number = *((uint16_t *) number);
    uint16_t philosophers_number_plus_one = philosophers_number + 1;
    uint16_t fork_num_plus_one = philosophers_number_plus_one % NUM_PHILOSOPHERS;
    
    while(1){
        // Think
        think(philosopher_names[philosophers_number]);
        
        // Get forks
        printf("%s waiting for forks %u and %u.\n", philosopher_names[philosophers_number], philosophers_number, fork_num_plus_one);
        fflush(stdout);
        
        sem_wait(&fork_semaphores[MIN(philosophers_number, fork_num_plus_one)]);
        sem_wait(&fork_semaphores[MAX(philosophers_number, fork_num_plus_one)]);
        
        printf("%s now has forks %u and %u.\n", philosopher_names[philosophers_number], philosophers_number, fork_num_plus_one);
        fflush(stdout);
        
        // Eat 
        eat(philosopher_names[philosophers_number]);
        philosopher_eat_counts[philosophers_number]++;
        
        // Release forks
        printf("%s releasing forks %u and %u.\n", philosopher_names[philosophers_number], philosophers_number, fork_num_plus_one);
        fflush(stdout);
        
        sem_post(&fork_semaphores[philosophers_number]);
        sem_post(&fork_semaphores[fork_num_plus_one]);
    
    }
    
}

int main(int argc, char **argv) {
    // Containers to hold our threads, and the philosopher index
    pthread_t consumer_threads[NUM_PHILOSOPHERS];
    uint16_t philosophers_numbers[NUM_PHILOSOPHERS];
    
    // Initialize semaphores, and arrays used to keep track of things
    for(uint16_t i = 0 ; i < NUM_PHILOSOPHERS ; i++){
        sem_init(&fork_semaphores[i], 0, 1);
        philosophers_numbers[i] = i;
        philosopher_eat_counts[i] = 0;
    }
    
    // Create all the philosopher threads
    for(uint16_t i = 0 ; i < NUM_PHILOSOPHERS ; i++){
        if(pthread_create(&consumer_threads[i], NULL, philosopher, &philosophers_numbers[i])) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }
    
    // This monitoring while loop prints our the eating counts of the 
    // philosophers so you can easily see how often they've ate without 
    // having to dig through the text output. It runs once every five seconds.
    while(1){
        sleep(5);
        
        printf("\n####################\nEat Counts\n");
        
        for(uint16_t i = 0 ; i < NUM_PHILOSOPHERS ; i++){
            printf("%s: %u\n", philosopher_names[i], philosopher_eat_counts[i]);
        }
        
        printf("####################\n\n");
        
        fflush(stdout);
        
    }
    
    return 0;
}


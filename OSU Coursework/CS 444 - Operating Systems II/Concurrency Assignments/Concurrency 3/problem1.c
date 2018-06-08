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
#define MAX_NUM_THREADS 4
#define MAX_RESOURCE_USAGE 3

#define MIN_RANDOM_USE_WAIT 3
#define MAX_RANDOM_USE_WAIT 5

//RDRAND value to check for
#define RDRAND_FLAG (1 << 30)

sem_t resource_semaphore;
uint8_t wait_for_resource_recovery = 0;

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


void resource_user_wait(uint8_t id, uint8_t *wait_kind){
    uint8_t random = get_random_number(MIN_RANDOM_USE_WAIT, MAX_RANDOM_USE_WAIT);
    
    printf("Resource user %d waiting %u seconds for %s.\n", id, random, wait_kind);
    fflush(stdout);
    
    sleep(random);
}

void *resource_user(void *thread_num){
    
    uint8_t thread_number = *((uint8_t *)thread_num);
    
    printf("Resourcing using thread with id: %d now running.\n", thread_number);
    fflush(stdout);
  
    while(1){
        resource_user_wait(thread_number, "initial wait");
        
        if(wait_for_resource_recovery){
            printf("Resource user %d waiting for recovery of all resources.\n", thread_number);
            fflush(stdout);
            while(wait_for_resource_recovery);
        }
        
        sem_wait(&resource_semaphore);
        
        printf("Resource user %d now using resource.\n", thread_number);
        fflush(stdout);
        
        resource_user_wait(thread_number, "usage wait");
        
        printf("Resource user %d done with resource.\n", thread_number);
        fflush(stdout);
        
        sem_post(&resource_semaphore);
    }
}

int main(int argc, char **argv) {
    // Containers to hold our threads, and the philosopher index
    pthread_t usage_threads[MAX_NUM_THREADS];
    uint8_t thread_id_numbers[MAX_NUM_THREADS];
    
    // Initialize semaphores, and arrays used to keep track of things
    sem_init(&resource_semaphore, 0, MAX_RESOURCE_USAGE);
    
    for(uint8_t i = 0 ; i < MAX_NUM_THREADS ; i++){
        thread_id_numbers[i] = i;
    }
    
    // Create all the philosopher threads
    for(uint8_t i = 0 ; i < MAX_NUM_THREADS ; i++){
        if(pthread_create(&usage_threads[i], NULL, resource_user, &thread_id_numbers[i])) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }
    
    int used_semaphores = 0;
    uint8_t resources_left = MAX_RESOURCE_USAGE;
    uint8_t last_resources_left_value = resources_left;
    
    
    while(1){
        sem_getvalue(&resource_semaphore, &used_semaphores);
        
        resources_left = MAX_RESOURCE_USAGE - used_semaphores;
        
        if(resources_left != last_resources_left_value){
                    
            printf("Manager ### Resources currently used: %d\n", resources_left);
            fflush(stdout);
            
            if(resources_left == MAX_RESOURCE_USAGE){
                printf("Manager ### Maximum resources used. Locking out.\n");
                fflush(stdout);
                wait_for_resource_recovery = 1;
            }else if (resources_left == 0){
                wait_for_resource_recovery = 0;
            }
            
            last_resources_left_value = resources_left;
        }  
    }
    
    return 0;
}


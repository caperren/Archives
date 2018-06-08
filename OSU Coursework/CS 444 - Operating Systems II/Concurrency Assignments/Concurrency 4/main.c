/* 
 * File:   main.c
 * Author: caperren
 *
 * Created on June 1st, 2018
 */
#define _POSIX_C_SOURCE 199309L

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
#include <time.h>

// Program defines you might want to change
#define NUM_BARBERSHOP_WAITING_ROOM_CHAIRS 5

// Program defines that don't get changed
#define NUM_CUSTOMER_THREADS (NUM_BARBERSHOP_WAITING_ROOM_CHAIRS * 2)

#define MIN_RANDOM_WAIT_CUSTOMER_RESPAWN 4
#define MAX_RANDOM_WAIT_CUSTOMER_RESPAWN 8

#define MIN_RANDOM_HAIRCUT_TIME 2
#define MAX_RANDOM_HAIRCUT_TIME 4

//RDRAND value to check for
#define RDRAND_FLAG (1 << 30)

// Global Variables 
sem_t waiting_room_chairs_semaphore;
sem_t ticket_dispenser_semaphore;
sem_t barber_chair_semaphore;

uint16_t current_ticket_number = 1;
uint16_t barber_servicing_ticket = 0;

uint8_t haircut_started = 0;
uint8_t haircut_over = 0;

struct timespec thread_sleep_time;

///////////////////////////////////////////////////////////////////
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

void get_hair_cut(uint16_t customer_thread_id, uint16_t ticket_id){
   
    while(!haircut_started){
        nanosleep(&thread_sleep_time, NULL);
    }
    
    printf("Customer heading to barbers chair with ticket number %u.\n", ticket_id);
    fflush(stdout);
    
    sem_wait(&barber_chair_semaphore);
    
    printf("Customer with thread id %u and ticket number %u sat down and getting haircut.\n", customer_thread_id, ticket_id);
    fflush(stdout);
    
    while(!haircut_over){
        nanosleep(&thread_sleep_time, NULL);
    }
    
    sem_post(&barber_chair_semaphore);
    
}

void cut_hair(){
    uint16_t sleep_time = get_random_number(MIN_RANDOM_HAIRCUT_TIME, MAX_RANDOM_HAIRCUT_TIME);
    int customer_in_chair = 1;
    
    haircut_over = 0;
    haircut_started = 1;
    
    printf("Barber waiting for customer to sit down with ticket number %d.\n", barber_servicing_ticket);
    fflush(stdout);
    while(customer_in_chair > 0){
        sem_getvalue(&barber_chair_semaphore, &customer_in_chair);
        nanosleep(&thread_sleep_time, NULL);
    
    }
    
    printf("Barber giving haircut to ticket number %d for %d seconds.\n", barber_servicing_ticket, sleep_time);
    fflush(stdout);
    
    sleep(sleep_time);
    
    haircut_started = 0;
    haircut_over = 1;
    
    while(customer_in_chair <= 0){
        sem_getvalue(&barber_chair_semaphore, &customer_in_chair);
        nanosleep(&thread_sleep_time, NULL);
    }
    
    printf("Barber finished haircut for ticket number %d.\n", barber_servicing_ticket);
    fflush(stdout);
}

// If waiting room is empty, sleep until woken up by customer
// If customer, cut_hair
void *barber(){
    printf("Barber thread started.\n");
    fflush(stdout);
    
    int used_waiting_room_chairs = 0;
    
    while(1){
        // Check if anyone is in the waiting room
        sem_getvalue(&waiting_room_chairs_semaphore, &used_waiting_room_chairs);
        
        if(used_waiting_room_chairs == 5){
            nanosleep(&thread_sleep_time, NULL);
            continue;
        }
        
        // Tell customers next ticket number
        barber_servicing_ticket++;
        
        // Service the customer
        cut_hair();
    }
}

// If the waiting room is full, customer leaves
// If waiting room has empty chair, sits in chair, unless barber is ready
// When ready, sits in chair and runs get_hair_cut, then leaves
void *customer(void *thread_id_number){
    uint16_t thread_id = *((uint16_t *)thread_id_number);

    printf("Customer thread with id %d started.\n", thread_id);
    fflush(stdout);
    
    int used_waiting_room_chairs = 0;
    uint16_t customer_ticket_number = 0;
    
    while(1){
        // Wait before deciding to enter store
        uint16_t sleep_time = get_random_number(MIN_RANDOM_WAIT_CUSTOMER_RESPAWN, MAX_RANDOM_WAIT_CUSTOMER_RESPAWN);        
        sleep(sleep_time);
        
        // Check if seats are full, exit store if full
        sem_getvalue(&waiting_room_chairs_semaphore, &used_waiting_room_chairs);
       
        if(used_waiting_room_chairs == 0){
            printf("Customer thread with id %d entered store, but was full. Leaving.\n", thread_id);
            fflush(stdout);
            continue;
        }
        
        // There's space in the waiting room, take a seat and a number
        sem_wait(&waiting_room_chairs_semaphore);
        sem_wait(&ticket_dispenser_semaphore);
        customer_ticket_number = current_ticket_number;
        current_ticket_number++;
        sem_post(&ticket_dispenser_semaphore);
        
        printf("Customer thread with id %d has ticket number %u.\n", thread_id, current_ticket_number);
        fflush(stdout);
        
        // Wait until our number is called
        while(barber_servicing_ticket != customer_ticket_number){
            nanosleep(&thread_sleep_time, NULL);
        }
        
        // It's time for a haircut, give up our seat in the waiting room
        sem_post(&waiting_room_chairs_semaphore);
        
        // Get a haircut
        get_hair_cut(thread_id, customer_ticket_number);        
    }
}

int main(int argc, char **argv) {
    // Setup thread sleep variable
    thread_sleep_time.tv_nsec = 10000000;
    thread_sleep_time.tv_sec = 0;
    
    // Storage for threads
    pthread_t barber_thread;
    pthread_t potential_customers_threads[NUM_CUSTOMER_THREADS];
    uint16_t customers_thread_ids[NUM_CUSTOMER_THREADS];
    
    
    // Initialize semaphores
    sem_init(&waiting_room_chairs_semaphore, 1, NUM_BARBERSHOP_WAITING_ROOM_CHAIRS);
    sem_init(&ticket_dispenser_semaphore, 1, 1);
    sem_init(&barber_chair_semaphore, 1, 1);
            
    // Start barber thread
    pthread_create(&barber_thread, NULL, barber, NULL);
    
    // Start potential custom threads
    for(uint16_t i = 0 ; i < NUM_CUSTOMER_THREADS ; i++){
        customers_thread_ids[i] = i;
        pthread_create(&potential_customers_threads[i], NULL, customer, &customers_thread_ids[i]);
    }
    
    // Wait for barber thread to finish
    pthread_join(barber_thread, NULL);
    
    // Wait for customer threads to finish
    for(uint8_t i = 0 ; i < NUM_CUSTOMER_THREADS ; i++){
        pthread_join(potential_customers_threads[i], NULL);
    }
}


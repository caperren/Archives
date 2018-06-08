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

////////// Program defines you might want to change //////////
#define NUM_INSERTERS 2
#define NUM_DELETERS 2
#define NUM_SEARCHERS 2

#define MIN_NUMBER 0
#define MAX_NUMBER 10

#define MIN_SLEEP_INSERTER 2
#define MAX_SLEEP_INSERTER 4

#define MIN_SLEEP_SEARCHER 1
#define MAX_SLEEP_SEARCHER 2

#define MIN_SLEEP_DELETER 10
#define MAX_SLEEP_DELETER 15

////////// Global variables and structs for linked list //////////
typedef struct linked_list {
    struct linked_list *next;
    uint16_t value;
} linked_list;

linked_list *list_head = NULL;
uint16_t list_size = 0;


////////// Global variables and structs for threading and sempaphores //////////
typedef struct light_switch {
    uint16_t counter;
    sem_t mutex;
} light_switch;


sem_t no_searches_semaphore;
sem_t no_insertions_semaphore;
sem_t insertion_mutex;

light_switch insertion_light_switch;
light_switch search_light_switch;


////////// Other global variables //////////
//RDRAND value to check for
#define RDRAND_FLAG (1 << 30)

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

void insert_linked_list(uint16_t value){
    linked_list *new = malloc(sizeof(linked_list));
    new->next = NULL;
    new->value = value;

    if(list_head == NULL){
        list_head = new;
    }else{
        linked_list *cursor = list_head;

        while(cursor->next != NULL){
            cursor = cursor->next;
        }
        cursor->next = new;
    }

    list_size++;
}

void delete_position_from_linked_list(uint16_t position) {
    linked_list *cursor_prev = list_head;
    linked_list *cursor_current = list_head;

    if(list_head == NULL || position >= list_size){
        return;
    }

    if(position == 0){
        list_head = cursor_current->next;
        free(cursor_current);
    }else{
        for(uint16_t i = 0 ; i < position ; i++){
            cursor_prev = cursor_current;
            cursor_current = cursor_current->next;
        }

        cursor_prev->next = cursor_current->next;
        free(cursor_current);
    }

    list_size--;
}

int16_t position_of_value_linked_list(uint16_t value){
    linked_list *cursor_current = list_head;

    if(list_head == NULL){
        return -1;
    }

    uint16_t count = 0;

    do{
        if(cursor_current->value == value){
            return count;
        }

        count++;
        cursor_current = cursor_current->next;
    }while(cursor_current != NULL);

    return -1;
}

void light_switch_lock(light_switch *current_light_switch, sem_t *current_semaphore){
    sem_wait(&current_light_switch->mutex);
    current_light_switch->counter++;

    if(current_light_switch->counter == 1){
        sem_wait(current_semaphore);
    }

    sem_post(&current_light_switch->mutex);
}

void light_switch_unlock(light_switch *current_light_switch, sem_t *current_semaphore){
    sem_wait(&current_light_switch->mutex);
    current_light_switch->counter--;

    if(current_light_switch->counter == 0){
        sem_post(current_semaphore);
    }

    sem_post(&current_light_switch->mutex);
}

void *inserter_thread(){
    printf("Inserter starting.\n");
    fflush(stdout);

    sleep(2); //Do this so you can easily see threads starting

    while(1){
        uint16_t new_number = get_random_number(MIN_NUMBER, MAX_NUMBER);
        uint16_t sleep_time = get_random_number(MIN_SLEEP_INSERTER, MAX_SLEEP_INSERTER);

        printf("Inserter going to sleep for %d seconds.\n", sleep_time);
        fflush(stdout);
        sleep(sleep_time);

        printf("Inserter awake and waiting for access.\n");
        fflush(stdout);
        light_switch_lock(&insertion_light_switch, &no_insertions_semaphore);
        sem_wait(&insertion_mutex);

        printf("Inserter now has access.\n");
        printf("Inserting number %u.\n", new_number);
        fflush(stdout);
        insert_linked_list(new_number);

        sem_post(&insertion_mutex);
        light_switch_unlock(&insertion_light_switch, &no_insertions_semaphore);
    }
}

void *searcher_thread(){
    printf("Searcher starting.\n");
    fflush(stdout);

    sleep(2); //Do this so you can easily see threads starting

    while(1){
        uint16_t search_value = get_random_number(MIN_NUMBER, MAX_NUMBER);
        uint16_t sleep_time = get_random_number(MIN_SLEEP_SEARCHER, MAX_SLEEP_SEARCHER);

        printf("Searcher going to sleep for %d seconds.\n", sleep_time);
        fflush(stdout);
        sleep(sleep_time);

        printf("Searcher awake and waiting for access.\n");
        fflush(stdout);
        light_switch_lock(&search_light_switch, &no_searches_semaphore);

        printf("Searcher has access.\n");
        fflush(stdout);

        int16_t value = position_of_value_linked_list(search_value);

        if(value != -1){
            printf("Search successful. Found value %d.\n", value);
        } else {
            printf("Search unsuccessful. Could not find value %u.\n", search_value);
        }
        fflush(stdout);

        light_switch_unlock(&search_light_switch, &no_searches_semaphore);

    }
}

void *deleter_thread(){
    printf("Deleter starting.\n");
    fflush(stdout);

    sleep(2); //Do this so you can easily see threads starting

    while(1){
        uint16_t sleep_time = get_random_number(MIN_SLEEP_DELETER, MAX_SLEEP_DELETER);

        printf("Deleter going to sleep for %d seconds.\n", sleep_time);
        fflush(stdout);
        sleep(sleep_time);

        printf("Deleter awake and waiting for access.\n");
        fflush(stdout);
        sem_wait(&no_searches_semaphore);
        sem_wait(&no_insertions_semaphore);

        uint16_t delete_position = get_random_number(0, list_size);

        printf("Deleter has access.\n");
        printf("Deleting node at position %d.\n", delete_position);
        fflush(stdout);
        delete_position_from_linked_list(delete_position);

        sem_post(&no_insertions_semaphore);
        sem_post(&no_searches_semaphore);
    }
}

int main(int argc, char **argv) {
    // Storage for threads
    pthread_t inserters[NUM_INSERTERS];
    pthread_t searchers[NUM_INSERTERS];
    pthread_t deleters[NUM_INSERTERS];

    // Initialize light switches
    sem_init(&insertion_light_switch.mutex, 1, 1);
    insertion_light_switch.counter = 0;

    sem_init(&search_light_switch.mutex, 1, 1);
    search_light_switch.counter = 0;

    // Initialize other semaphores and mutex
    sem_init(&no_insertions_semaphore, 1, 1);
    sem_init(&no_searches_semaphore, 1, 1);
    sem_init(&insertion_mutex, 1, 1);

    // Spin up threads
    for(uint8_t i = 0 ; i < NUM_INSERTERS ; i++){
        pthread_create(&inserters[i], NULL, inserter_thread, NULL);
    }

    for(uint8_t i = 0 ; i < NUM_SEARCHERS ; i++){
        pthread_create(&searchers[i], NULL, searcher_thread, NULL);
    }

    for(uint8_t i = 0 ; i < NUM_DELETERS ; i++){
        pthread_create(&deleters[i], NULL, deleter_thread, NULL);
    }

    // Wait for threads to join for completion.
    for(uint8_t i = 0 ; i < NUM_INSERTERS ; i++){
        pthread_join(inserters[i], NULL);
    }

    for(uint8_t i = 0 ; i < NUM_SEARCHERS ; i++){
        pthread_join(searchers[i], NULL);
    }

    for(uint8_t i = 0 ; i < NUM_DELETERS ; i++){
        pthread_join(deleters[i], NULL);
    }
}

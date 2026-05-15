/*
Austin Nguyen
CSEN177 Lab 5
Description: Program to solve producer-consumer problem using semaphores with buffer
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 10
pthread_t threads[BUFFERSIZE];
int in;
int out;
int buffer[BUFFERSIZE];

sem_t *lock;
sem_t *empty;
sem_t *full;

void *consumer(void* arg){
    do{
        sem_wait(full);             // When buffer is full
        sem_wait(lock);             // Critical Section entry pt
        int item = buffer[out];     // takes item out of buffer
        out = (out + 1) % BUFFERSIZE;
        sem_post(lock);             // Exit Crit Section
        sem_post(empty);
        printf("\tConsumed Item: %d\n", item);
        sleep(1);
    }while(1);
}

void *producer(void *arg){
    do{
        int item = rand() % 100;     // Create Item
        sem_wait(empty);            // Checks if Consumer emptied buffer
        sem_wait(lock);             // Entry
        buffer[in] = item;          // Insert into Buffer
        in = (in + 1) % BUFFERSIZE;
        sem_post(lock);
        sem_post(full);
        printf("Produced Item: %d\n", item);
        sleep(1);
    }while(1);
}

int main(){
    int i;
    lock = sem_open("Lock", O_CREAT, 0644, 1);
    empty = sem_open("Empty", O_CREAT, 0644, BUFFERSIZE);
    full = sem_open("Full", O_CREAT, 0644, 0);
    in = out = 0;
    for(i = 0; i < BUFFERSIZE / 2; ++i){            // Producers
        pthread_create(&threads[i], NULL, producer, (void *)(size_t)i);
    }
    for(i = BUFFERSIZE / 2 ; i < BUFFERSIZE; ++i){  // Consumers
        pthread_create(&threads[i], NULL, consumer, (void *)(size_t)i);
    }
    for(i = 0; i < BUFFERSIZE; ++i){                // Thread Termination
        pthread_join(threads[i], NULL);
    }
    sem_unlink("lock");
    sem_unlink("full");
    sem_unlink("empty");
    return 0;
}
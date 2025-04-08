/*
Austin Nguyen
CSEN177 Lab 5
Description: Program to solve producer-consumer problem using semaphores with buffer
this time using condition variables and keeping track of how full buffer is
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define NP 3
#define NC 2
#define n 10
pthread_t tidP[NP], tidC[NC];
int buffer[n];
pthread_mutex_t lock;
pthread_cond_t full, empty;

int p, c, in, out;
int count = 0;

void* producer(void* arg){
    do{
        int item;
        item = rand() %100;
        while(count == n){
            pthread_cond_wait(&empty, &lock);
        }
        buffer[in] = item;
        in = (in + 1) % n;
        count++;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
        printf("Produced: %d\n", item);
        sleep(1);
    }while(1);
}

void* consumer(void* arg){
    do{
        pthread_mutex_lock(&lock);
        while(count == 0){
            pthread_cond_wait(&full, & lock);
        }
        int item = buffer[out];
        out = (out + 1) % n;
        count --;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&lock);
        printf("\tConsumed: %d\n", item);
        sleep(1);
    }while(1);
}

int main(){
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&full, NULL);
    for(p = 0; p < NP; ++p){
        pthread_create(&tidP[p], NULL, producer, (void*)(size_t)p);
    }
    for(c = 0; c < NC ; c++){
        pthread_create(&tidC[c], NULL, consumer, (void*)(size_t)c);
    }
    for(p = 0; p < NP; p++){
        pthread_join(tidP[p], NULL);
        printf("Producer Thread %d returned\n", p);
    }
    for(c = 0; c < NC; c++){
        pthread_join(tidC[c], NULL);
        printf("Consumer Thread %d returned\n", c);
    }
    printf("Main Thread Finished.\n");
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&empty);
    pthread_cond_destroy(&full);
    return 0;
}
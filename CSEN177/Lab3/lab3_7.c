/*
Austin Nguyen
10/19/24
Lab 3 Part 7
Description: Same code as lab 6 with an added temp ptr to fix the printing errors in
step 6
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *go(void *);
#define NTHREADS 10
pthread_t threads[NTHREADS];

int main(){
    int i;
    for (i = 0; i < NTHREADS; ++i){
        int *tempadd = malloc(sizeof(*tempadd));
        *tempadd = i;
        pthread_create(&threads[i], NULL, go, tempadd);
    }
    for ( i = 0; i < NTHREADS; i++){
        printf("Thread %d returned\n", i);
        pthread_join(threads[i],NULL);
    }
    printf("Main thread done.\n");
    return 0;
}

void *go(void *arg){
    printf("Hello from thread %d with iteration %d\n", (int)pthread_self(), *(int *)arg);
    free(arg);
    return 0;
}
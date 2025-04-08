#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

void *secondth(void *vptr){
    int i;
    for (i = 0; i < 100; i++){
        printf("Second Thread %d \n", i);
        usleep(3000);
    }
}

int main(){
    int i;
    pthread_t thread;
    if(pthread_create(&thread, NULL, &secondth, NULL) != 0){
        perror("pthread_create() error.\n");
        exit(1);
    }
    for (i = 0; i < 100; i++){
        printf("First Thread: %d\n", i);
        usleep(3000);
    }
    return 0;
}
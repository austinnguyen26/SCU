// Thread Sychronization
//To compile this code:
//If using the SCU Linux lab first add #include <fcntl.h>
//You may need to add -lpthread to your gcc command like:
//gcc threadSync.c -lpthread
//gcc threadSync.c -o test -lpthread


#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 

#define NTHREADS 10
pthread_t threads[NTHREADS];
sem_t *lock; 

void* go(void* arg) { 
  sem_wait(lock); //entry section
  printf("Thread %d Entered Critical Section..\n", (int)(size_t)arg); //critical section 
   sleep(1); 
  sem_post(lock); //exit section 
  return (NULL);
} 

int main() { 
lock = sem_open("lock", O_CREAT, 0644, 1);
static int i;
for (i = 0; i < NTHREADS; i++)  
   pthread_create(&threads[i], NULL, go, (void *)(size_t)i);
for (i = 0; i < NTHREADS; i++) {
   pthread_join(threads[i],NULL);
   printf("\t\t\tThread %d returned \n", i);
   }
printf("Main thread done.\n");
sem_unlink("lock"); 
return 0; 
} 

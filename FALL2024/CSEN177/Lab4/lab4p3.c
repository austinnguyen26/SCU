/*
Austin Nguyen | anguyen10@scu.edu
Lab 4 Part 3: Using Threading to Perform Matrix Multiplication
Description: This program creates 2 3x3 matrixes (random elements from 0-10) and 
multiplies them together and stores them into a third matrix using threading.
*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define mSIZE 3

double m1[mSIZE][mSIZE], m2[mSIZE][mSIZE], m3[mSIZE][mSIZE];
pthread_t multithread[mSIZE];

void *multiplication(void *);

int main(){
    int i;
    int j;
    srand(time(NULL));
    printf("Matrix 1:\n");
    for(i = 0; i < mSIZE; ++i){
        for(j = 0; j < mSIZE; ++j){
            m1[i][j] = rand() % 10;
            printf("%.1f\t", m1[i][j]);
        }
        printf("\n");
    }
    printf("Matrix 2: \n");
    for(i = 0; i < mSIZE; ++i){
        for(j = 0; j < mSIZE; ++j){
            m2[i][j] = rand() % 10;
            printf("%.1f\t", m2[i][j]);
        }
        printf("\n");
    }
    for(i = 0; i < mSIZE; ++i){
        pthread_create(&multithread[i], NULL, multiplication, (void *)(size_t)i);
    }
    for(i = 0; i < mSIZE; ++i){
        pthread_join(multithread[i], NULL);
    }
    // printf("Main Thread Done\n");

    printf("Matrix 3 (The product of M1 and M2)\n");
    for (i = 0; i< mSIZE; ++i){
        for (j = 0; j < mSIZE; ++j){
            printf("%.1f\t", m3[i][j]);
        }
        printf("\n");
    }
    printf("Main Thread Done\n");
}


void *multiplication(void *arg){
    int c;                              //collumn
    int r = (int)(size_t)arg;           //row
    int k;
    for(c = 0; c < mSIZE; ++c){
        double sum = 0;
        for(k = 0; k < mSIZE; ++k){
            sum += m1[r][k] * m2[k][c];
        }
        m3[r][c] = sum; 
    }
    return NULL;
}

/*
bool foundInCache = false;
for (i=0; i<cacheSize; i++){
if (cache[i].pageno == page_num){
foundInCache = true;
break; //break out loop because found page_num in cache
}
}
if (foundInCache == false){
//You may print the page that caused the page fault
cache[placeInArray].pageno = page_num;
totalFaults++
placeInArray++; //Need to keep the value within the cacheSize
*/
// Austin Nguyen
// November 23, 2024
// Lab 8: fifo.c
// Description: First in First out Algorithm implementation for part 4.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int pageno;
} ref_page;

int main (int argc, char *argv[]){
    int cacheSize = atoi(argv[1]);
    ref_page cache[cacheSize];
    char inputBuffer[100];
    int i;
    int totalFaults = 0;
    int countRequests = 0;
    int placeInArray = 0;
    for ( i = 0; i < cacheSize; ++i){
        cache[i].pageno = -1;
    }
    while(fgets(inputBuffer, 100, stdin)){
        countRequests++;
        int page_num = atoi(inputBuffer);

        bool foundInCache = false;
        for (i=0; i<cacheSize; i++){
            if (cache[i].pageno == page_num){
            foundInCache = true;
            break; //break out loop because found page_num in cache
            }
        }
        if (foundInCache == false){
            printf("%d\n", page_num);
            cache[placeInArray].pageno = page_num;
            totalFaults++;
            placeInArray++; //Need to keep the value within the cacheSize
            if(placeInArray == cacheSize){
                placeInArray = 0;
            }
        }
    }
    float hitRate = ((float)(countRequests - totalFaults)/(float)countRequests);
    fprintf(stderr,"%d: Number of Requests\n", countRequests);
    fprintf(stderr,"%d: Number of Page Faults\n", totalFaults);
    fprintf(stderr,"%f: Hit Rate\n", hitRate);
    return 0;
}
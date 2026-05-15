// Austin Nguyen
// November 23, 2024
// Lab 8: secondchance.c
// Description: Second Chance Algorithm implementation for part 4.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int pageno;
    int second_chance;
} ref_page;

int main (int argc, char *argv[]){
    int cacheSize = atoi(argv[1]);
    ref_page cache[cacheSize];
    char pageCache[100];
    int i;
    int totalFaults = 0;
    int countRequests = 0;
    int counter;
    for( i = 0; i < cacheSize; ++i){
        cache[i].pageno = -1;
        cache[i].second_chance = 0;
    }
    while(fgets(pageCache, 100, stdin)){
        countRequests++;
        int page_num = atoi(pageCache);
        
        bool foundInCache = false;
        for (i=0; i<cacheSize; i++){
            if (cache[i].pageno == page_num){
                foundInCache = true;
                cache[i].second_chance = 1;
                break; //break out loop because found page_num in cache
            }
        }
        if (foundInCache == false){
            printf("%d\n", page_num);
            totalFaults++;
            for(i = counter; i < cacheSize; ++i){
                if(cache[i].second_chance == 0){
                    cache[i].pageno = page_num;
                    counter++;
                    if(counter == cacheSize){
                        counter = 0;
                    }
                    break;
                }else{
                    cache[i].second_chance = 0;
                    counter++;
                    if(counter == cacheSize){
                        i=-1;
                        counter = 0;
                    }
                }
            }
        }
    }
    float hitRate = ((float)(countRequests - totalFaults)/(float)countRequests);
    fprintf(stderr,"%d: Number of Requests\n", countRequests);
    fprintf(stderr,"%d: Number of Page Faults\n", totalFaults);
    fprintf(stderr,"%f: Hit Rate\n", hitRate);
    return 0;
}
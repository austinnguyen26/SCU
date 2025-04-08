// Austin Nguyen
// November 23, 2024
// Lab 8: lru.c
// Description: Least Recently Used Algorithm implementation for part 4.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int pageno;
    int recent_access;
} ref_page;


int main(int argc, char *argv[]){
    int cacheSize = atoi(argv[1]); 
    ref_page cache[cacheSize]; 
    char pageCache[100]; 
    int i;
    int totalFaults = 0; 
    int totalChecks = 0;
    int totalRequests = 0;
    
    for (i = 0; i < cacheSize; i++){ 
        cache[i].pageno = -1;
        cache[i].recent_access = -1;
    }

    while (fgets(pageCache, 100, stdin)){
        totalRequests++;
        int page_num = atoi(pageCache);

        bool foundInCache = false;
        for (i=0; i<cacheSize; i++){ 
            if (cache[i].pageno == page_num){
                foundInCache = true;
                cache[i].recent_access = totalChecks;
                break;
            }
        }   
        if (foundInCache == false){ 
            printf("%d\n", page_num);
            totalFaults++;

            int minAccess = 2147483647;
            int minIndex;
            for (i=0; i<cacheSize; i++){ 
                if (cache[i].recent_access < minAccess){
                    minAccess = cache[i].recent_access;
                    minIndex = i;
                }
            }      
            cache[minIndex].pageno = page_num;
            cache[minIndex].recent_access = totalChecks;
        }
        totalChecks++;
    }

    float hitRate = ((float)(totalRequests - totalFaults)/(float)totalRequests);
    fprintf(stderr,"%d: Number of Requests\n", totalRequests);
    fprintf(stderr,"%d: Number of Page Faults\n", totalFaults);
    fprintf(stderr,"%f: Hit Rate\n", hitRate);
    return 0;
}
// Austin Nguyen
// November 23, 2024
// Lab 8: skeleton.c
// Description: Given .c file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
    int pageno;
    //Any other useful values associated with the page, e.g. referenced
} ref_page;


int main(int argc, char *argv[]){
    int cacheSize = atoi(argv[1]); // Size of Cache passed by user
    ref_page cache[cacheSize]; // Virtual cache for simulator 
    char inputBuffer[100]; // Buffer to hold input from test file

    int i;
    int totalFaults = 0; // keeps track of the total page faults
    
    for (i = 0; i < cacheSize; i++){//initialise cache array  
         cache[i].pageno = -1;
    }

    while (fgets(inputBuffer, 100, stdin)){
    	int page_num = atoi(inputBuffer); // Stores number read from file as an int

    	/*

		
		Page Replacement Implementation Goes Here 


    	*/
    }

    return 0;
}

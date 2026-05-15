/*
* Name: Austin Nguyen
* Date: 1/13/25 (Monday 2:15 PM - 5:00 PM)
* Title: Lab2 – Threads and HTTP
* Description: C program that calculates response time for a web client receiving
* objects from a server. The program considers various RTTs for DNS look-ups, 
* HTTP requests, and responses.
*/


#include <stdio.h>
#define NUM_OBJECTS 6
#define PARALLEL_CONNECTIONS 6

int main() {
    // Given RTT values in milliseconds
    double RTT0 = 3;    // Local DNS cache
    double RTT1 = 20;   // First external DNS server
    double RTT2 = 26;   // Second external DNS server
    double RTT_HTTP = 47; // HTTP server RTT


    // (a) Total time for DNS lookup and receiving the base HTML object
    double total1 = RTT0 + RTT1 + RTT2 + (2*RTT_HTTP) ;
    printf("a. Total time to receive the base object: %.2f ms\n", total1);

    // (b) Total time for DNS lookup and receiving all objects with non-persistent HTTP
    double total2 = total1 + (NUM_OBJECTS * RTT_HTTP) * 2;
    printf("b. Total time for Non Persistent 6 Objects: %.2f ms\n", total2);

    // (c) Time calculation with persistent and non-persistent HTTP with n parallel connections
    double total3 = total1 + RTT_HTTP * NUM_OBJECTS / PARALLEL_CONNECTIONS;
    printf("c. Total Time for Persistent: %.2f ms\n", total3);

    double total4 = total1 + RTT_HTTP * 2 * NUM_OBJECTS / PARALLEL_CONNECTIONS;
    printf("Total Time for non persistent: %.2f ms\n", total4);

    return 0;
}

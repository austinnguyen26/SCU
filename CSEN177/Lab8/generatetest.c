// Austin Nguyen
// November 23, 2024
// Lab 8: generatetest.c
// Description: .c file to generate testInput.txt
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int numRequests = 100; // Adjust as necessary
    int possiblePages = 50; // Adjust as necessary
    FILE *fp = fopen("testInput.txt", "w");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    for (int i = 0; i < numRequests; i++) {
        fprintf(fp, "%d\n", rand() % possiblePages);
    }

    fclose(fp);
    return 0;
}
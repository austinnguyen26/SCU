#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    char buffer[10000];
    FILE *fp;
    fp = fopen(argv[1], "rb");
    while (fread(buffer, sizeof(buffer), 1, fp)){
    }
    fclose(fp);
}

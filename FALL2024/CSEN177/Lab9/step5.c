#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>

#define N 64

pthread_t threads[N];
int bufferSize;
char file[9];
FILE *fp;
char* temp;
void *copyFile(void *args)
{
    FILE *fp1;
    FILE *fp2;
    char buff[0x100];
    snprintf(buff, sizeof(buff), "copy%d.out", (int) (size_t) args);

    fp1 = fopen(buff, "wb");
    fp2 = fopen(temp, "rb");
    int sizeOfBuffer = bufferSize;
    char buf[sizeOfBuffer];

    while(fread(&buf, sizeof(buf), 1, fp2))
    {

        fwrite(buf, sizeof(buf), 1, fp1);
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}

int main(int argc, char *argv[])
{
    temp = argv[1];
    if (argc != 4)
    {
        fprintf(stderr, "Usage %s <testingFile> <buffer size> <Number of Threads> \n", argv[0]);
        return 1;
    }

    
    
    bufferSize = atoi(argv[2]);
    strncpy(file, argv[1], 9);


    int i;
    for(i = 0; i < atoi(argv[3]); i++)
    {
        pthread_create(&threads[i], NULL, copyFile, (void *)(size_t) i);
    }

    for(i = 0; i < atoi(argv[3]); i++)
    {
        pthread_join(threads[i], NULL);
    }


    return 0;
}
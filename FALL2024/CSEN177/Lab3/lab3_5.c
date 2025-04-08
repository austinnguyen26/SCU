/*
Austin Nguyen
10/19/24
Lab 3 Part 5
Description: Producer, Consumer message communication function with pipes
that exits when inputting 'exit'
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main (int argc, char *argv[]){
    int fds[2];
    pipe(fds);
    char buff[60];
    int count;
    if (fork() == 0){
        close(fds[0]);
        char input[60];
        while(1){
            sleep(1);
            printf("Producer: (Type 'exit' to break program)\n");
            scanf("%s", input);
            write(fds[1], input, strlen(input)+1);
            if(strcmp(input, "exit")== 0){
                break;
            }
        }
        close(fds[1]);
        exit(0);
    }else if (fork() == 0){
        close (fds[1]);
        while(1){
            //sleep(1);
            count = read(fds[0], buff, sizeof(buff));
            if(count > 0 ){
                printf("Consumer received msg: %s\n", buff);
                if (strcmp(buff, "exit")==0){
                    break;
                }
            }
        }
        close(fds[0]);

        exit(0);
    }else{
        close(fds[0]);
        close(fds[1]);
        wait(0);
        wait(0);
    }
    return 0;
}
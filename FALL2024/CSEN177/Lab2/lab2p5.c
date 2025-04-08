#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

int main (int argc, char *argv[]) {
    pid_t pid;
    int i;
    int gen = 0;
    int numchildren = 0;
    int n = atoi(argv[1]);
    bool make_children = true;

//need the while loop to keep iterating or else wont print anything out
    while(1){
        pid = fork();
        if(pid == 0){
            numchildren = 0;
            gen ++;
            if(gen >=3){     //only a 3 layer tree | Root, Parent, Children
                printf("ChildID: %d, ParentID: %d\n", getpid(), getppid());
                break;
            }
            if(make_children){
                continue;
            }else{
                printf("ChildID: %d, ParentID: %d\n", getpid(), getppid());
                break;
            }
        }else{
            numchildren++;
            make_children = false;
            if(numchildren == 2){
                printf("ChildID1: %d, ChildID2: %d, Parent ID: %d, Parent ID: %d\n", wait(0), wait(0), getpid(), getppid());
                exit(0);
            }
        }
    }
    return 0;
}
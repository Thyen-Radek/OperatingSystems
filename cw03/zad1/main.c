#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <ftw.h>

int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr,"Wrong number of arguments");
        exit(1);
    }
    pid_t child_pid;
    int number = atoi(argv[1]);
    printf("PID of main program: %d\n", (int)getpid());
    fflush(stdout);
    for(int i = 0;i<number;i++){

        child_pid = fork();
        if(child_pid==0) {
            printf("Parent process PID: %d\n",(int)getppid());
            printf("Child process PID: %d\n",(int)getpid());
            exit(0);
        }
        wait(NULL);

    }
    printf("Number of arguments: %s ",argv[1]);
    
    exit(0);
}
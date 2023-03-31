#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    if (argc != 2){
        fprintf(stderr,"Wrong number of arguments!");
        exit(1);
    }
    char *expression;
    expression = argv[1];
    if(strcmp(expression,"ignore") == 0){
        printf("%s \n","Raising signal in child process");
        raise(SIGUSR1);
    }else if(strcmp(expression,"mask") == 0){
        printf("%s \n","Raising signal in child process");
        raise(SIGUSR1);

        sigset_t sig_set;

        sigemptyset(&sig_set);

        sigpending(&sig_set);

        if(sigismember(&sig_set,SIGUSR1)){
            printf("Signal is waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }else{
            printf("Signal is not waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }
    }else if(strcmp(expression,"pending") == 0){
        sigset_t sig_set;

        sigemptyset(&sig_set);

        sigpending(&sig_set);

        if(sigismember(&sig_set,SIGUSR1)){
            printf("Signal is waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }else{
            printf("Signal is not waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

int main(int argc,char *argv[]){
    if (argc != 2){
        fprintf(stderr,"Wrong number of arguments!");
        exit(1);
    }
    char *expression;
    expression = argv[1];
    if(strcmp(expression,"ignore") == 0){
        printf("%s \n","Ignore testing...");
        signal(SIGUSR1, SIG_IGN);
        raise(SIGUSR1);
        fflush(stdout);
        if (execl("./child", "child", argv[1], NULL) < 0) {
            fprintf(stdout, "Error occured using funciton exec!");
            exit(1);
        }
    }else if (strcmp(expression,"mask") == 0 || strcmp(expression,"pending") == 0){
        if(strcmp(expression,"mask") == 0){
            printf("%s \n","Mask testing...");
        }else{
            printf("%s \n","Pending testing...");
        }
        sigset_t sig_set;

        sigemptyset(&sig_set);

        sigaddset(&sig_set,SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &sig_set, NULL) < 0){
            perror("Couldn't block signal");         
        }
        raise(SIGUSR1);

        sigpending(&sig_set);

        if(sigismember(&sig_set,SIGUSR1)){
            printf("Signal is waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }else{
            printf("Signal is not waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }
        fflush(stdout);
        if (execl("./child", "child", argv[1], NULL) < 0) {
            fprintf(stdout, "Error occured using funciton exec!");
            exit(1);
        }
        
    }
}
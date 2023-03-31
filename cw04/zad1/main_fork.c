#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>

void handler(int sig){
    printf("Handler, pid: %d, parent pid: %d, signal: %d \n",getpid(),getppid(),sig);
}

int main(int argc, char *argv[]) {
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
        if(fork() == 0) {
            printf("%s \n","Raising signal in child process");
            raise(SIGUSR1);
            
        }
        wait(NULL);
    }else if (strcmp(expression,"handler") == 0){
        printf("%s \n","Handler testing...");
        signal(SIGUSR1, handler);
        raise(SIGUSR1);
        fflush(stdout);
        if(fork() == 0) {
            printf("%s \n","Raising signal in child process");
            raise(SIGUSR1);
        }
        wait(NULL);

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
        // signal(SIGUSR1,handler);
        raise(SIGUSR1);

        sigpending(&sig_set);

        if(sigismember(&sig_set,SIGUSR1)){
            printf("Signal is waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }else{
            printf("Signal is not waiting, ppid: %d, pid: %d \n", getppid(),getpid());
        }
        fflush(stdout);
        if(fork() == 0) {
            if(strcmp(expression,"mask") == 0){
                printf("%s \n","Raising signal in child process");
                raise(SIGUSR1);
            }

            sigpending(&sig_set);
            if(sigismember(&sig_set,SIGUSR1)){
                printf("Signal is waiting, ppid: %d, pid: %d \n", getppid(),getpid());
            }else{
                printf("Signal is not waiting, ppid: %d, pid: %d \n", getppid(),getpid());
            }
        }
        wait(NULL);

    }else{
        fprintf(stderr,"Wrong argument!!");
        exit(1);
    }
   
    exit(0);
}
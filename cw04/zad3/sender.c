#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

void handler1(int sig) {
    printf("Sender received SIGUSR1 signal from catcher\n");
}

int main(int argc,char *argv[]){
     if (argc < 3) {
        perror("Wrong number of arguments\n");
        exit(1);
    }
    int pid;
    

    if ((pid = atoi(argv[1])) < 1) {
        perror("Invalid PID catcher_PID\n");
        exit(1);
    }

    printf("Sender with PID: %d\n", getpid());

    struct sigaction act;
    union sigval value;
    act.sa_handler = handler1;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGUSR1, &act, NULL) == -1){
        perror("Sigaction error\n");
    }

    sigset_t sigset;

    sigaddset(&act.sa_mask, SIGUSR1);

    sigemptyset(&sigset);

    int index = 2;
    
    while (index < argc) {
        if ((value.sival_int = atoi(argv[index])) < 1) {
            perror("Invalid value \n");
            exit(1);
        }
        sigqueue(pid, SIGUSR1, value);
        sigsuspend(&sigset);

        index += 1;
    }
    return 0;
}
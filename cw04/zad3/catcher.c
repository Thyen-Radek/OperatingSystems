#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

void handler1(int sig, siginfo_t *info, void *ucontext) {
    printf("Catcher received SIGUSR1 signal from sender\n");
}

int main(int argc,char *argv[]){
    
    printf("Catcher with PID: %d waiting for SIGUSR1\n", getpid());

    struct sigaction act;
    act.sa_sigaction = handler1;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGUSR1, &act, NULL) == -1){
        perror("Sigaction error\n");
    }
   
    // sigdelset(&signal_set, SIGUSR1);
    while (1);
}
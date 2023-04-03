#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

int counter = 0;
int flag = 0;

void current_time() {
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );
    printf("Current local time and date: %s", asctime(info));
}

void handler1(int sig, siginfo_t *info, void *ucontext) {
    printf("Catcher received SIGUSR1 signal from sender %d\n", info->si_pid);
    if(info->si_value.sival_int == 1){
        flag = 0;
        for (int i = 1; i < 101; i++){
            printf("%d ", i);
        }
        printf("\n");
    } else if(info->si_value.sival_int == 2){
        flag = 0;
        current_time();
    }else if(info->si_value.sival_int == 3){
        flag = 0;
        printf("Number of requests: %d\n", counter);
    }else if(info->si_value.sival_int == 4){
        flag = 1;
        printf("Showing time...\n");
    }else if(info->si_value.sival_int == 5){
        printf("Exiting...\n");
        kill(info->si_pid, SIGUSR1);
        exit(0);
    }else{
        printf("Wrong argument received: %d!",info->si_value.sival_int);
    }
    counter++;
    kill(info->si_pid, SIGUSR1);
}

int main(){
    
    printf("Catcher with PID: %d waiting for SIGUSR1\n", getpid());

    struct sigaction act;
    act.sa_sigaction = handler1;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGUSR1, &act, NULL) == -1){
        perror("Sigaction error\n");
    }
   
    // sigdelset(&signal_set, SIGUSR1);
    while (1){
        if (flag){
            current_time();
        }
        sleep(1);
    }
}
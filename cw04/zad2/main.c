#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

void siginfo_handler(int sig_no,siginfo_t *info, void *context){
    printf("Siginfo handler!\n"); 
    printf("Signal number from sig: %d, from siginfo %d\n", sig_no, info->si_signo);
    printf("PID: %d, PPID: %d \n",info->si_pid,getppid());
    printf("User ID of sending that process: %d \n",info->si_uid);
    printf("Code identifying the cause of the signal : %d\n", info->si_code);
}
void nocldstop_handler(int sig_no,siginfo_t *info, void *context){
    printf("Nocldstop handler!\n"); 
    printf("Signal number from sig: %d,PID: %d, PPID: %d\n",sig_no,info->si_pid,getppid());
}
void resethand_handler(int sig_no){
    printf("Resethand handler!\n"); 
    printf("Signal number from sig: %d,PID: %d, PPID: %d\n",sig_no,getpid(),getppid());
}
void child_test(int sig_no){
    pid_t pid;
    
    if((pid = fork()) == 0){
        while(1);
    }else {
        sleep(1);
        printf("Process %d is sending %d to %d\n", getppid(), sig_no, pid);
        kill(pid, sig_no);
        sleep(1);
    }
}
void siginfo_test(){
    printf("--Testing sa_sigaction--\n");

    struct sigaction act;
    
    act.sa_sigaction = siginfo_handler; 
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0;
    if(sigaction(SIGINT, &act, NULL) == -1){
        perror("Sigaction error \n");
    }
    raise(SIGINT);
    printf("Setting flag sa_siginfo!\n");
    act.sa_flags = SA_SIGINFO;
    if(sigaction(SIGINT, &act, NULL) == -1){
        perror("Sigaction error \n");
    }
    raise(SIGINT);
}

void nocldstop_test(){
    printf("--Testing sa_nocldstop--\n");

    struct sigaction act;
    act.sa_sigaction = nocldstop_handler;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0;
    if(sigaction(SIGCHLD, &act, NULL) == -1){
        perror("Sigaction error \n");
    }

    child_test(SIGSTOP);
    printf("Setting flag sa_nocldstop!\n");
    act.sa_flags = SA_NOCLDSTOP;
    if(sigaction(SIGCHLD, &act, NULL) == -1){
        perror("Sigaction error \n");
    }
    child_test(SIGSTOP);
}
void resethand_test(){
    printf("--Testing sa_resethand--\n");

    struct sigaction act;
    
    act.sa_handler = resethand_handler; 
    sigemptyset(&act.sa_mask); 
    act.sa_flags = 0;
    if(sigaction(SIGCHLD, &act, NULL) == -1){
        perror("Sigaction error \n");
    }
    child_test(SIGSTOP);
    printf("Setting flag sa_resethand!\n");
    act.sa_flags = SA_RESETHAND;
    child_test(SIGSTOP);
}
int main(){

    // SIGINFO 
    siginfo_test();

    // NOCLDSTOP
    nocldstop_test();

    // RESETHAND
    resethand_test();
    
}
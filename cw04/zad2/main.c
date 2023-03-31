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
    
    exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <limits.h>

int main(int argc,char *argv[]){
    if (argc != 3){
        fprintf(stderr,"Wrong number of arguments \n");
        exit(1);
    }
    return 0;
}
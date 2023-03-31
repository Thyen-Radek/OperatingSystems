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

    setbuf(stdout, NULL);

    printf("%s ",argv[0]);

    fflush(stdout);
    execl("/bin/ls","ls",argv[1],NULL);

    exit(0);
}
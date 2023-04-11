#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

double calculate_function(double x){
    return (double)4/(x*x+(double)1);
}

int main(int argc,char *argv[]){
    int n = atoi(argv[2]);
    double distance = strtod(argv[1], (char **)NULL);
    int i = atoi(argv[3]);

    char *path;
    path = "/tmp/pipe";

    double range = (double)1/(double)n;
    double a = range*i;
    double b = range*(i+1);
    double sum = 0;
    for (double j = a; j < b;j+= distance){
        sum += calculate_function(j);
    }

    int fd = open(path,O_WRONLY);
    write(fd,&sum,sizeof(sum));
    close(fd);

    exit(0);
}
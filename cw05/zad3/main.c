#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


clock_t start,end;
struct tms start_time_tms,end_time_tms;

void printer(char *processes,char *distance, FILE *dest_file){
    printf("Distance: %s number of processes: %s \n",distance,processes);
    printf("Real time of function: %fs\n",((double) (end-start))/ sysconf(_SC_CLK_TCK));
    printf("User time of function: %fs\n",((double) (end_time_tms.tms_utime-start_time_tms.tms_utime))/ sysconf(_SC_CLK_TCK));
    printf("System time of function: %fs\n",((double) (end_time_tms.tms_stime-start_time_tms.tms_stime))/ sysconf(_SC_CLK_TCK));
    fprintf(dest_file,"Distance: %s number of processes: %s \n",distance,processes);
    fprintf(dest_file,"Real time of function: %fs\n",((double) (end-start))/ sysconf(_SC_CLK_TCK));
    fprintf(dest_file,"User time of function: %fs\n",((double) (end_time_tms.tms_utime-start_time_tms.tms_utime))/ sysconf(_SC_CLK_TCK));
    fprintf(dest_file,"System time of function: %fs\n",((double) (end_time_tms.tms_stime-start_time_tms.tms_stime))/ sysconf(_SC_CLK_TCK));
}

double calculate_function(double x){
    return (double)4/(x*x+(double)1);
}

int main(int argc,char *argv[]){
    if (argc != 3){
        fprintf(stderr,"Wrong number of arguments \n");
        exit(1);
    }
    int n = atoi(argv[2]);
    double distance = strtod(argv[1], (char **)NULL);
    if (distance <= 0 || distance > 1){
        fprintf(stderr,"Length of rectangle must be positive and lesser than 1 \n");
        exit(1);
    }
    if(n <= 0){
        fprintf(stderr,"Number of proccesses must be positive \n");
        exit(1);
    }
    printf("Distance: %f \n",distance);
    char *path;
    path = "/tmp/pipe";
    start = times(&start_time_tms);
    if (mkfifo(path, 0666) < 0){
        if (errno != EEXIST){
            fprintf(stderr,"Error occured during pipe \n");
            exit(1);
        }
    }
    int fd;
    double sum = 0;
    for(int i = 0; i < n;i++){
        pid_t pid = fork();


        if(pid == 0){
            char str[i+1];
            sprintf(str, "%d", i);
            execl("./solve","./solve",argv[1],argv[2],str,NULL);
        } else{
            fd = open(path, O_RDONLY);
            if (fd < 0){
                fprintf(stderr,"Error occured during reading from pipe \n");
                exit(1);
            }
            double res;
            read(fd,&res,sizeof(res));
            sum+= res;
        }
    }
    sum = sum * distance;
    end = times(&end_time_tms);

    FILE *out_file;
    out_file = fopen("raport.txt", "a");
    
    printer(argv[2],argv[1],out_file);

    printf("Result of function: %f \n",sum);
    fprintf(out_file,"Result of function: %f \n\n", sum);

    fclose(out_file);

    exit(0);
}
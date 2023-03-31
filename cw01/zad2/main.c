#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "../zad1/library.h"
#include <dlfcn.h>

// https://pubs.opengroup.org/onlinepubs/009695299/functions/times.html
void printer(clock_t start,clock_t end, struct tms start_time_tms,struct tms end_time_tms,char function_name[]){
    printf("Real time of function %s : %fs\n",function_name ,((double) (end-start))/ sysconf(_SC_CLK_TCK));
    printf("User time of function %s : %fs\n",function_name ,((double) (end_time_tms.tms_utime-start_time_tms.tms_utime))/ sysconf(_SC_CLK_TCK));
    printf("System time of function %s : %fs\n",function_name ,((double) (end_time_tms.tms_stime-start_time_tms.tms_stime))/ sysconf(_SC_CLK_TCK));
}

int main(){
    #ifdef DYNAMIC
    void *handle = dlopen("../zad1/library.so", RTLD_LAZY);

    void (*set_table)(int);
    void (*wc_counting)(char **);
    char * (*get_from_index)(int);
    void (* remove_block)(int);
    void (*remove_table)(void);

    set_table = dlsym(handle,"set_table");
    wc_counting = dlsym(handle,"wc_counting");
    get_from_index = dlsym(handle,"get_from_index");
    remove_block = dlsym(handle,"remove_block");
    remove_table = dlsym(handle,"remove_table");
    #endif

    clock_t start_clock_t, end_clock_t;
    struct tms start_tms_t, end_tms_t;
    // struct timespec start,end;
    static int line_length = 1000;
    int size,index;
    char *line = calloc(line_length,sizeof(char));
    char * command;
    while(fgets(line,line_length,stdin)){
        // if(strcmp(line," ") != 0 && strcmp(line,"\n") != 0){
        command = strtok(line," \n");
        if(command != NULL){
            if(strcmp(command,"init") == 0){
                // clock_gettime(CLOCK_REALTIME,&start);
                start_clock_t = times(&start_tms_t);
                command = strtok(NULL," \n");
                if(command == NULL){
                    fprintf(stderr, "Wrong number of arguments!");
                    exit(1);
                }
                size = atoi(command);
                if(size < 1){
                    fprintf(stderr, "Wrong size");
                    exit(1);
                }
                set_table(size);
                // clock_gettime(CLOCK_REALTIME,&end);
                end_clock_t = times(&end_tms_t);
                char *function_name = "init";
                // printf("Real %fs",(double)((end.tv_sec-start.tv_sec) + (end.tv_nsec-start.tv_nsec)));
                printer(start_clock_t,end_clock_t,start_tms_t,end_tms_t,function_name);
            }
            else if(strcmp(command,"count") == 0){
                // clock_gettime(CLOCK_REALTIME,&start);
                start_clock_t = times(&start_tms_t);
                command = strtok(NULL," \n");
                if(command == NULL){
                    fprintf(stderr, "Wrong number of arguments!");
                    exit(1);
                }
                wc_counting(&command);
                // clock_gettime(CLOCK_REALTIME,&end);
                end_clock_t = times(&end_tms_t);
                char *function_name = "count";
                // printf("Real %fs",(double)((end.tv_sec-start.tv_sec) + (end.tv_nsec-start.tv_nsec)));
                printer(start_clock_t,end_clock_t,start_tms_t,end_tms_t,function_name);
            }
            else if(strcmp(command,"show") == 0){
                // clock_gettime(CLOCK_REALTIME,&start);
                start_clock_t = times(&start_tms_t);
                command = strtok(NULL," \n");
                if(command == NULL){
                    fprintf(stderr, "Wrong number of arguments!");
                    exit(1);
                }
                index = atoi(command);
                char *result = get_from_index(index);
                if(result != NULL){
                    printf("Index: %d, Having: %s",index,result);
                }
                // clock_gettime(CLOCK_REALTIME,&end);
                end_clock_t = times(&end_tms_t);
                char *function_name = "show";
                printer(start_clock_t,end_clock_t,start_tms_t,end_tms_t,function_name);
            }
            else if(strcmp(command,"delete") == 0){
                // clock_gettime(CLOCK_REALTIME,&start);
                start_clock_t = times(&start_tms_t);
                command = strtok(NULL," \n");
                if(command == NULL){
                    fprintf(stderr, "Wrong number of arguments!");
                    exit(1);
                }
                index = atoi(command);
                remove_block(index);
                // clock_gettime(CLOCK_REALTIME,&end);
                end_clock_t = times(&end_tms_t);
                char *function_name = "delete";
                printer(start_clock_t,end_clock_t,start_tms_t,end_tms_t,function_name);
            }
            else if(strcmp(command,"destroy") == 0){
                // clock_gettime(CLOCK_REALTIME,&start);
                start_clock_t = times(&start_tms_t);
                remove_table();
                // clock_gettime(CLOCK_REALTIME,&end);
                end_clock_t = times(&end_tms_t);
                char *function_name = "destroy";
                printer(start_clock_t,end_clock_t,start_tms_t,end_tms_t,function_name);
            }
            else if(strcmp(command,"exit") == 0){
                printf("Exiting...\n");
                exit(0);
            }
        }        
    }
}
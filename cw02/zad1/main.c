#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <string.h>
#include <ctype.h>

static clock_t start,end;
static struct tms start_time_tms,end_time_tms;

void printer(char function_name[], FILE *dest_file){
    printf("Real time of function %s : %fs\n",function_name ,((double) (end-start))/ sysconf(_SC_CLK_TCK));
    printf("User time of function %s : %fs\n",function_name ,((double) (end_time_tms.tms_utime-start_time_tms.tms_utime))/ sysconf(_SC_CLK_TCK));
    printf("System time of function %s : %fs\n",function_name ,((double) (end_time_tms.tms_stime-start_time_tms.tms_stime))/ sysconf(_SC_CLK_TCK));
    fprintf(dest_file,"Real time of function %s : %fs\n",function_name ,((double) (end-start))/ sysconf(_SC_CLK_TCK));
    fprintf(dest_file,"User time of function %s : %fs\n",function_name ,((double) (end_time_tms.tms_utime-start_time_tms.tms_utime))/ sysconf(_SC_CLK_TCK));
    fprintf(dest_file,"System time of function %s : %fs\n",function_name ,((double) (end_time_tms.tms_stime-start_time_tms.tms_stime))/ sysconf(_SC_CLK_TCK));
}

void lib_func(char *filename_find, char *filename_out, char *char_search, char *char_change){
    char c;
    FILE *file_find,*file_out;
    if ((file_find = fopen(filename_find, "r")) == NULL || (file_out = fopen(filename_out, "w")) == NULL) {
        fprintf(stderr, "Cannot open a file");
        exit(1);
    }
    while(!feof(file_find) && fread(&c,sizeof(char),1,file_find) == 1){
        if(c == *char_search){
            fwrite(char_change,sizeof(char),1,file_out);
        }
        else {
            fwrite(&c,sizeof(char),1,file_out);
        }
    }
    fclose(file_find);
    fclose(file_out);
}

void sys_func(char *filename_find, char *filename_out, char *char_search, char *char_change){
    char c;
    int file_find,file_out;
    if ((file_find = open(filename_find, O_RDONLY)) < 0 || (file_out = open(filename_out, O_WRONLY)) < 0) {
        fprintf(stderr, "Cannot open a file");
        exit(1);
    }
    while(read(file_find, &c, 1) == 1){
        if(c == *char_search){
            write(file_out, char_change, 1);
        }
        else {
            write(file_out, &c, 1);
        }
    }
    close(file_find);
    close(file_out);
}

int main(int argc, char *argv[]) {
    char *filename_find, *filename_out, *char_search, *char_change;
    filename_find = calloc(256, sizeof(char));
    filename_out = calloc(256, sizeof(char));
    char_search = calloc(2,sizeof(char));
    char_change = calloc(2,sizeof(char));
    if (argc < 5){
        fprintf(stderr,"Not enough arguments!");
        exit(1);
    } else if (argc > 5) {
        fprintf(stderr,"Wrong number of arguments (you need 4) !");
        exit(1);
    }
    filename_find = realloc(filename_find,strlen(argv[3]) + 1);
    filename_out = realloc(filename_out,strlen(argv[4]) + 1);
    strcpy(filename_find,argv[3]);
    strcpy(filename_out,argv[4]);
    strcpy(char_search,argv[1]);
    strcpy(char_change,argv[2]);

    FILE *out_file;
    out_file = fopen("pomiar_zad_1.txt", "w");

    start = times(&start_time_tms);
    lib_func(filename_find,filename_out,char_search,char_change);
    end = times(&end_time_tms);

    printer("lib",out_file);

    start = times(&start_time_tms);
    sys_func(filename_find,filename_out,char_search,char_change);
    end = times(&end_time_tms);

    printer("sys",out_file);

    fclose(out_file);

    free(filename_find);
    free(filename_out);
    free(char_search);
    free(char_change);

}
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

void lib_func_first(char *filename_find, char *filename_out){
    char *c;
    c = calloc(1,sizeof(char));
    
    FILE *file_find,*file_out;
    if ((file_find = fopen(filename_find, "r")) == NULL || (file_out = fopen(filename_out, "w")) == NULL) {
        fprintf(stderr, "Cannot open a file");
        exit(1);
    }
    fpos_t index;
    fseek(file_find,-1,SEEK_END);
 
    fgetpos(file_find, &index);
    while(index != 0){
        fgetpos(file_find, &index);
        fread(c,sizeof(char),1,file_find);
        fwrite(c,sizeof(char),1,file_out);
        fseek(file_find,-2,SEEK_CUR);
    }
  
    fclose(file_find);
    fclose(file_out);
}
void lib_func_second(char *filename_find, char *filename_out){
    char *c;
    int size = 1024;
    c = calloc(size,sizeof(char));
    
    FILE *file_find,*file_out;
    if ((file_find = fopen(filename_find, "r")) == NULL || (file_out = fopen(filename_out, "w")) == NULL) {
        fprintf(stderr, "Cannot open a file");
        exit(1);
    }
    fpos_t index;
    fseek(file_find,0,SEEK_END);
    fgetpos(file_find, &index);
    int while_size = size;
    // printf("%lld \n",index);
    while(index != 0){
        if(index < size){
            while_size = index;
        }
        fseek(file_find,-while_size,SEEK_CUR);
        fgetpos(file_find, &index);
        // printf("%lld \n",index);
        
        fread(c,sizeof(char),while_size,file_find);
        for(int i = while_size-1;i >= 0;i--){
            fwrite(&c[i],sizeof(char),1,file_out);
        }
        fseek(file_find,-while_size,SEEK_CUR);
        if(index < while_size - 1){
            while_size = index;
        }
    }
    fclose(file_find);
    fclose(file_out);
}

int main(int argc, char *argv[]) {
    char *filename_find, *filename_out;
    filename_find = calloc(256, sizeof(char));
    filename_out = calloc(256, sizeof(char));

    if (argc < 3){
        fprintf(stderr,"Not enough arguments!");
        exit(1);
    } else if (argc > 5) {
        fprintf(stderr,"Wrong number of arguments (you need 4) !");
        exit(1);
    }
    filename_find = realloc(filename_find,strlen(argv[1]) + 1);
    filename_out = realloc(filename_out,strlen(argv[2]) + 1);
    strcpy(filename_find,argv[1]);
    strcpy(filename_out,argv[2]);


    FILE *out_file;
    out_file = fopen("pomiar_zad_2.txt", "w");

    start = times(&start_time_tms);
    lib_func_first(filename_find,filename_out);
    end = times(&end_time_tms);

    printer("1_char",out_file);

    start = times(&start_time_tms);
    lib_func_second(filename_find,filename_out);
    end = times(&end_time_tms);

    printer("1024_chars",out_file);

    fclose(out_file);

    free(filename_find);
    free(filename_out);

}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <dirent.h>

void dir_sum(char path[]){
    long long sum_size = 0;
    static char *new_path;
    struct stat bufor_stat;
    struct dirent *directory_kids;
    DIR *directory;
    int n;
    
    new_path = calloc(1000,sizeof(char));
    if ((directory = opendir(path)) == NULL) {
        fprintf(stderr, "Cannot read directory");
    }
    n = strlen(path);
    
    strcpy(new_path,path);
    new_path[n++] = '/';

    while ((directory_kids = readdir(directory)) != NULL) {
        if(strcmp(directory_kids->d_name,".") == 0 || strcmp(directory_kids->d_name,"..") == 0){
            continue;
        }
        strcpy(&new_path[n],directory_kids->d_name);
        stat(new_path,&bufor_stat);
        if(!S_ISDIR(bufor_stat.st_mode)){
            sum_size += bufor_stat.st_size;
            printf("Filename: %s - "
            "Filesize: %lld \n",directory_kids->d_name, bufor_stat.st_size);
        }
    }
    printf("Summarized size: %lld \n", sum_size);
    if (closedir(directory) < 0) {
        fprintf(stderr, "Cannot close directory");
        exit(1);
    }
}

int main(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        fprintf(stderr,"Can't load directory");
        exit(1);
    }
    dir_sum(cwd);
}
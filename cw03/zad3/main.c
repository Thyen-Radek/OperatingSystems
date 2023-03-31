#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

void search_file(char *path,char *find_string){
    FILE *file;
    char *file_buff;
    if(strlen(path) > PATH_MAX){
        perror("Too long path!");
        exit(1);
    }
    if ((file = fopen(path, "r")) == NULL) {
        perror("Couldn't load file!");
        exit(1);
    }
    // printf("%s \n", path);
    int n_string;
    n_string = strlen(find_string);
    // printf("%d ", n_string);
    file_buff = calloc(n_string,sizeof(char));

    if(!fread(file_buff,sizeof(char),n_string,file)){
        perror("Couldn't read file");
        exit(1);
    }
    // printf("%s \n", find_string);
    // printf("%s \n", file_buff);
    if(strcmp(file_buff,find_string) == 0){
        printf("%s %d \n", path, getpid());
    }
    free(file_buff);
    fclose(file);
}

void search_path(char *path,char *find_string){
    DIR *dp;
    struct stat statbuf;
    struct dirent *dirp;
    int n;
    n = strlen(path);
    path[n++] = '/';
    path[n] = 0;

    if(strlen(path) >= PATH_MAX){
        perror("Too long path!");
        exit(1);
    }

    if ((dp = opendir(path)) == NULL) {
        perror("Couldn't open directory!");
        exit(1);
    }
    
    while ((dirp = readdir(dp)) != NULL){
        if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0){
            continue;
        }
        strcpy(&path[n],dirp->d_name);
        // printf("%s \n",path);
        stat(path,&statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if(fork() == 0){
                search_path(path,find_string);
                exit(0);
            }
            wait(NULL);
        }
        else {
            search_file(path,find_string);
        }
    }
    // printf("%s ", path);
    // printf("%s ", find_string);
    return;
}

int main(int argc, char *argv[]) {
    if (argc != 3){
        perror("Wrong number of arguments");
        exit(1);
    }   
    printf("Proces programu: %d \n",getpid());
    fflush(stdout);
    char *path,*find_string;
    path = calloc(PATH_MAX,sizeof(char));
    realpath(argv[1],path);
    if(strlen(path) > PATH_MAX){
        perror("Too long path!");
        exit(1);
    }
    find_string = calloc(255,sizeof(char));
    if(strlen(argv[2]) > 255){
        perror("Too long string!");
        exit(1);
    }
    strcpy(find_string, argv[2]);
    search_path(path,find_string);

    free(path);
    free(find_string);
    exit(0);
}
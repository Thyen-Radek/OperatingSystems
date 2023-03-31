#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <ftw.h>

static long long sum_size;

int status(const char *fpath, const struct stat *sb, int typeflag) {
    if(typeflag == FTW_F || !S_ISDIR(sb->st_mode)){
        printf("%lld ",sb->st_size);
        printf("%s \n",fpath);
        sum_size += sb->st_size;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments");
        exit(1);
    }
    if(ftw(argv[1],status,10)==-1){
        fprintf(stderr, "Can't load directory");
        exit(1);
    }
    printf("Summarized size: %lld \n", sum_size);
    
}
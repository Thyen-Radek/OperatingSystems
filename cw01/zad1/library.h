#ifndef lab1_lib
#define lab1_lib

typedef struct blockMem {
    int size;
    char *value;
} blockMem ;

typedef struct myArray {
    blockMem **block;
    int max_size; 
    int cur_size;
} myArray ;

void set_table(int size);

void wc_counting(char *file[]);

char *get_from_index(int index);

void remove_block(int index);

void remove_table();

#endif
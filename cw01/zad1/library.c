#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

static myArray *table;

void set_table(int size){
    printf("Array initialization...\n");
    table = calloc(1,sizeof(myArray));
    table->max_size = size;
    table->cur_size = 0;
    table->block = calloc(size,sizeof(blockMem));
    
}

void wc_counting(char *file[]) {
    printf("Saving file...\n");
    if (table == NULL){
        fprintf(stderr,"You have to initialize array first!\n");
        return;
    }
    char *command = (char*) calloc(1000,sizeof(char));
    // printf("%s",*file);
    strcat(command,"wc ");
    strcat(command, *file);
    strcat(command, " > tmp.txt");
    int s_command = system(command);
    if (s_command == -1){
        fprintf(stderr,"Command failed!\n");
        return;
    }
    free(command);
    
    FILE *tmp;
    tmp = fopen("tmp.txt","r");

    fseek(tmp, 0, SEEK_END);
    int file_size = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);

    int index = 0;
    while (table->block[index] != NULL && index < table->max_size){
        index += 1;
    }
    // printf("%d %d",index,file_size);

    table->block[index] = calloc(1,sizeof(blockMem));
    table->block[index]->value = calloc(file_size ,sizeof(char));
    table->block[index]->size = file_size;

    int s_fread = fread( table->block[index]->value, sizeof(char), file_size, tmp );
    if (s_fread == -1){
        fprintf(stderr,"Allocation failed!\n");
        return;
    }
    fclose(tmp);

    table->cur_size++;

    int s_remove = system("rm -rf tmp.txt");
    if (s_remove == -1){
        fprintf(stderr,"Remove failed!\n");
        return;
    }
    // printf(index);
    // printf(table->block[index]->value);
    // printf("XD");
}
char * get_from_index(int index){
    printf("Getting from block...\n");
    if(index >= 0 && index < table->max_size && table->block[index] != NULL){
        return table->block[index]->value;
    }
    else {
        fprintf(stderr,"No data saved on index or wrong index!\n");
        return NULL;
    }
    
}

void remove_block(int index){
    printf("Removing from block...\n");
    if(table->block[index] == NULL){
        fprintf(stderr,"Block is already freed!\n");
        return;
    }
    if(table->max_size <= index || index < 0){
        fprintf(stderr,"Wrong index!\n");
        return;
    }
    free(table->block[index]->value);
    // table->block[index]->size = NULL;
    free(table->block[index]);
    table->block[index] = NULL;
}

void remove_table(){
    printf("Freeding table...\n");
    if(table == NULL){
        fprintf(stderr,"Array is already freed!\n");
        return;
    }
    for(int index=0; index<table->max_size;index++){
        if(table->block[index] != NULL){
            free(table->block[index]->value);
            // table->block[index]->size = NULL;
            free(table->block[index]);
            table->block[index] = NULL;
        }
    }
    free(table->block);
    // table->cur_size = NULL;
    // table->max_size = NULL;
    free(table);
    table = NULL;
}

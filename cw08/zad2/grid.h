#pragma once
#include <stdbool.h>
#include <signal.h>

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst);
void* update_grid_thread(void* args);
void sig_handler(int signo, siginfo_t *info, void *context);
void destroy_threads();
void update_grid_multithreaded(char* src, char* dst, int threads_num);

typedef struct {
    char *src;
    char *dst;
    int start;
    int end;
} thread_args_t;


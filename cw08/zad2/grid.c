#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;
static pthread_t*threads = NULL;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

void*update_grid_thread(void *args)
{
    thread_args_t *t_args = (thread_args_t *)args;
   
    while (true)
    {
        for (int i = t_args->start; i < t_args->end; ++i)
        {
            t_args->dst[i] = is_alive(i/grid_width, i%grid_width, t_args->src);
        }
        pause();

        char*tmp = t_args->src;
        t_args->src = t_args->dst;
        t_args->dst = tmp;
    }
    return NULL;
    
}
void sig_handler(int signo, siginfo_t *info, void *context){}

void destroy_threads(){
    free(threads);
}

void update_grid_multithreaded(char *src, char *dst, int n_threads)
{
    static int default_threads = grid_height * grid_width;

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    
    if (!threads){

        n_threads = n_threads < 0 ? default_threads : n_threads;
        
        threads = malloc(sizeof(pthread_t) * n_threads);

        int block = default_threads / n_threads + (default_threads % n_threads == 0 ? 0 : 1);

        for (int i = 0; i < default_threads; i+=block)
        {
            // It will free memory once program is terminated
            thread_args_t *args = malloc(sizeof(thread_args_t));
            args->src = src;
            args->dst = dst;
            args->start = i;
            args->end = i + block > default_threads ? default_threads : i + block;

            pthread_create(threads + i/block, NULL, update_grid_thread, args);
        }
    }
    for (int i = 0; i < n_threads; i++)
    {
        pthread_kill(threads[i], SIGUSR1);
    }
}
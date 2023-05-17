#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdbool.h>

#define REINDEER_TOTAL 9
#define ELVES_TOTAL 10
#define ELVES_REQUIRED 3

static pthread_barrier_t reindeer_barrier, elves_barrier;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    unsigned int reindeer_count;
    unsigned int elves_count;
} checker;

static checker santa_checker = {
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER,
    .reindeer_count = 0,
    .elves_count = 0
};

pid_t get_t_id(void) {
    return syscall(SYS_gettid);
}

void *reindeer(void *arg){
    while (true) {
        printf("Reindeer %d is on vacation\n", get_t_id());
        sleep(5 + (rand() % 6));

        pthread_mutex_lock(&santa_checker.mutex);

        santa_checker.reindeer_count++;
        if (santa_checker.reindeer_count == REINDEER_TOTAL){
            printf("Reindeer %d is waking up Santa\n", get_t_id());
        }else {
            printf("Reindeer %d is waiting for other reindeer\n", get_t_id());
        }
        pthread_cond_signal(&santa_checker.cond);
       
        pthread_mutex_unlock(&santa_checker.mutex);
        pthread_barrier_wait(&reindeer_barrier);
    }

    return NULL;
}

void *elf(void *arg){
    while(true){
        sleep(2 + rand() % 4);

        pthread_mutex_lock(&santa_checker.mutex);

        if (santa_checker.elves_count < ELVES_REQUIRED){
            santa_checker.elves_count++;
            if (santa_checker.elves_count == ELVES_REQUIRED){
                printf("Elf %d is waking up Santa\n", get_t_id());
            }else {
                printf("Elf %d is waiting for other elves\n", get_t_id());
            }
            pthread_cond_signal(&santa_checker.cond);
        }else {
            printf("Elf %d is resolving a problem\n", get_t_id());
            pthread_mutex_unlock(&santa_checker.mutex);
            continue;
        }
        pthread_mutex_unlock(&santa_checker.mutex);
        pthread_barrier_wait(&elves_barrier);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_barrier_init(&reindeer_barrier, NULL, REINDEER_TOTAL + 1);
    pthread_barrier_init(&elves_barrier, NULL, ELVES_REQUIRED + 1);

    pthread_t reindeer_threads[REINDEER_TOTAL];
    pthread_t elf_threads[ELVES_TOTAL];

    for (int i = 0; i < REINDEER_TOTAL; ++i) {
        pthread_create(&reindeer_threads[i], NULL, reindeer, NULL);
    }
    for (int i = 0; i < ELVES_TOTAL; ++i) {
        pthread_create(&elf_threads[i], NULL, elf, NULL);
    }
    while (true) {
        pthread_mutex_lock(&santa_checker.mutex);

        while(santa_checker.reindeer_count < REINDEER_TOTAL && santa_checker.elves_count < ELVES_REQUIRED){
            pthread_cond_wait(&santa_checker.cond, &santa_checker.mutex);
        }
        
        if(santa_checker.reindeer_count == REINDEER_TOTAL){
            printf("Santa woke up, delivering presents\n");
            sleep(2 + rand() % 3);
            santa_checker.reindeer_count = 0;
            pthread_barrier_wait(&reindeer_barrier);
        }
        else if(santa_checker.elves_count == ELVES_REQUIRED){
            printf("Santa woke up, helping elves\n");
            sleep(1 + rand() % 2);
            santa_checker.elves_count = 0;
            pthread_barrier_wait(&elves_barrier);
        }
        else {
            printf("Santa woke up, but there is nothing to do\n");
        }
        pthread_mutex_unlock(&santa_checker.mutex);

    }

    return 0;
}
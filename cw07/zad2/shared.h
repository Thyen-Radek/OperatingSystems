#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>

#define PROJECT getenv("HOME")
#define QUEUE_NAME "0"
#define CHAIRS_NAME "1"
#define HAIRDRESSERS_NAME "2"

#define HAIRDRESSER 3
#define CHAIRS 4
#define QUEUE_SIZE 6
#define CUSTOMERS 5
#define CUT_TIME 1000
#define BUFFER_SIZE 4096
#define WAITING_TIME 1000000

typedef enum haircuts {
    A = 1000,
    B = 2000,
    C = 3000,
    D = 4000,
    E = 5000
} haircuts; 

char *shared_memory(const char* filename, int size){
    int descriptor = shm_open(filename, O_RDWR | O_CREAT, 0666);
    if(descriptor == -1){
        perror("shm_open");
        return NULL;
    }
    if (ftruncate(descriptor, size) == -1){
        perror("ftruncate");
        return NULL;
    }
    char *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);
    return mem;
}

bool release_shared_memory(const char* filename){
    if (shm_unlink(filename) == -1){
        perror("shm_unlink");
        return false;
    }
    return true;
}

sem_t* create_semaphore(const char* filename, int value){
    sem_t* s = sem_open(filename, O_RDWR | O_CREAT, 0666, value);
    if (s == SEM_FAILED){
        perror("sem_open");
        exit(1);
    }
    return s;
}

sem_t* open_semaphore(const char* filename){
    sem_t* s = sem_open(filename, 0);
    if (s == SEM_FAILED){
        perror("sem_open");
        exit(1);
    }
    return s;
}

void add_to_queue(char *queue, char byte){
    if (strlen(queue) + 1 >= QUEUE_SIZE){
        printf("Queue is full");
        return;
    }
    int size = strlen(queue);
    queue[size] = byte;
    queue[size + 1] = '\0';
}
char get_from_queue(char *queue){
    if (strlen(queue) == 0){
        printf("Queue is empty");
        return '\0';
    }
    char byte = queue[0];
    memcpy(queue, queue + 1, strlen(queue));
    return byte;
}

void lock_semaphore(sem_t* semid){
    if (sem_wait(semid) == -1){
        perror("sem_wait");
        exit(1);
    }
}
void unlock_semaphore(sem_t* semid){
    if (sem_post(semid) == -1){
        perror("sem_post");
        exit(1);
    }
}
int get_semaphore_value(sem_t* semid){
    int value;
    if(sem_getvalue(semid, &value) == -1){
        perror("semctl");
        exit(1);
    }
    return value;
}
void close_semaphore(sem_t* semid){
    if(sem_close(semid) == -1){
        perror("sem_close");
        exit(1);
    }
}
void unlink_semaphore(char *filename){
    if(sem_unlink(filename) == -1){
        perror("sem_unlink");
        exit(1);
    }
}



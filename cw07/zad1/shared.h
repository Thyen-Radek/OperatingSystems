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
    key_t key = ftok(filename, 0);
    if(key == -1){
        perror("ftok");
        exit(1);
    }

    int shmid = shmget(key, size, 0666 | IPC_CREAT);
    if(shmid == -1){
        perror("shmget");
        exit(1);
    }

    char *mem = shmat(shmid, NULL, 0);
    if(mem == (char*)-1){
        perror("shmat");
        exit(1);
    }

    return mem;
    
}

bool release_shared_memory(const char* filename, char *mem){
    key_t key = ftok(filename, 0);
    if(key == -1){
        perror("ftok");
        return false;
    }

    int shmid = shmget(key, 0, 0666 | IPC_CREAT);
    if(shmid == -1){
        perror("shmget");
        return false;
    }

    if(shmdt(mem) == -1){
        perror("shmdt");
        return false;
    }

    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        perror("shmctl");
        return false;
    }

    return true;
}

int create_semaphore(const char* filename, int value){
    key_t key = ftok(getenv("HOME"), filename[0]);
    if(key == -1){
        perror("ftok");
        exit(1);
    }

    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if(semid == -1){
        perror("semget");
        exit(1);
    }

    if(semctl(semid, 0, SETVAL, value) == -1){
        perror("semctl");
        exit(1);
    }
    return semid;
}

int open_semaphore(const char* filename){
    key_t key = ftok(getenv("HOME"), filename[0]);
    if(key == -1){
        perror("ftok");
        exit(1);
    }

    int semid = semget(key, 1, 0666);
    if(semid == -1){
        perror("semget");
        exit(1);
    }
    return semid;
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

void lock_semaphore(int semid){
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    if(semop(semid, &sops, 1) == -1){
        perror("semop");
        exit(1);
    }
}
void unlock_semaphore(int semid){
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    if(semop(semid, &sops, 1) == -1){
        perror("semop");
        exit(1);
    }
}
int get_semaphore_value(int semid){
    int value = semctl(semid, 0, GETVAL);
    if(value == -1){
        perror("semctl");
        exit(1);
    }
    // printf("Semaphore %d value: %d\n", semid, value);
    return value;
}
void set_semaphore_value(int semid, int value){
    if(semctl(semid, 0, SETVAL, value) == -1){
        perror("semctl");
        exit(1);
    }
}



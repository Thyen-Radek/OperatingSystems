#include "shared.h"

int sem_queue;
int sem_chairs;
int sem_hairdressers;

void get_semaphores();
char haircut();
int main(void){
    srand(time(NULL) + getpid());
    // Queue memory
    char *shared = shared_memory(PROJECT,BUFFER_SIZE);
    if (shared == NULL) {
        printf("Error while accessing shared memory\n");
        exit(1);
    }

    get_semaphores();

    if (strlen(shared) == QUEUE_SIZE){
        printf("Queue is full, customer is leaving...\n");
        exit(1);
    }

    // Customer is added to queue and then he waits for his turn
    lock_semaphore(sem_queue);

    char haircut_type = haircut();
    printf("Client started with pid: %d and haircut type: %c\n", getpid(), haircut_type);
    fflush(stdout);

    add_to_queue(shared, haircut_type);

    
    
    unlock_semaphore(sem_hairdressers);
    printf("Client is invited by hairdresser\n");

    // Unlinking shared memory
    shmdt(shared);

    return 0;
}

void get_semaphores(){
    sem_queue = open_semaphore(QUEUE_NAME);
    sem_chairs = open_semaphore(CHAIRS_NAME);
    sem_hairdressers = open_semaphore(HAIRDRESSERS_NAME);
}
char haircut(){
    // Random haircut
    return (char) (rand() % 26 + 65);
}
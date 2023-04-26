#include "shared.h"


sem_t* sem_queue;
sem_t* sem_chairs;
sem_t* sem_hairdressers;

void create_semaphores();
void close_semaphores();
void unlink_semaphores();

int main(void){
    printf("Simulation started\n");
    fflush(stdout);

    char *shared = shared_memory(PROJECT,BUFFER_SIZE);
    if (shared == NULL) exit(1);

    shared[0] = '\0';
    printf("Shared memory %s created\n", shared);

    create_semaphores();
    for (int i = 0; i < HAIRDRESSER; i++){
        pid_t pid = fork();
        if (pid == 0){
            char *id = malloc(sizeof(char) * 10);
            sprintf(id, "%d", i+1);
            execl("./hairdresser", "./hairdresser",id, NULL);
        }
    }
    printf("Hairdressers created\n");
    fflush(stdout);

    for (int i = 0; i < CUSTOMERS; i++){
        pid_t pid = fork();
        if (pid == 0){
            execl("./client", "./client", NULL);
        }
    }
    printf("Clients created\n");
    fflush(stdout);

    while (wait(NULL) > 0);

    printf("Simulation ended\n");
    fflush(stdout);
    if (release_shared_memory(PROJECT) != 1){
        printf("Error while releasing shared memory\n");
        exit(1);
    }
    printf("Shared memory released\n");
    fflush(stdout);

    close_semaphores();
    printf("Semaphores closed\n");
    fflush(stdout);

    unlink_semaphores();
    printf("Semaphores unlinked\n");
    fflush(stdout);

    return 0;

}
void unlink_semaphores(){
    unlink_semaphore(QUEUE_NAME);
    unlink_semaphore(CHAIRS_NAME);
    unlink_semaphore(HAIRDRESSERS_NAME);
}
void close_semaphores(){
    close_semaphore(sem_queue);
    close_semaphore(sem_chairs);
    close_semaphore(sem_hairdressers);
}
void create_semaphores(){
    sem_queue = create_semaphore(QUEUE_NAME, QUEUE_SIZE);
    sem_chairs = create_semaphore(CHAIRS_NAME, CHAIRS);
    sem_hairdressers = create_semaphore(HAIRDRESSERS_NAME, 0);
}
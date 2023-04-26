#include "shared.h"

int sem_queue;
int sem_chairs;
int sem_hairdressers;
int hairdresser_id;
int cut_time;

void get_semaphores();
void set_hair_time(char haircut_type);

int main(int argc, char *argv[]){
    hairdresser_id = atoi(argv[1]);
    // Queue memory
    char *shared = shared_memory(PROJECT,BUFFER_SIZE);
    if (shared == NULL) {
        printf("Error while accessing shared memory\n");
        exit(1);
    }

    get_semaphores();
    
    printf("Hairdresser started with pid: %d and id: %d\n", getpid(), hairdresser_id);
    fflush(stdout);

    while (true)
    {     
        printf("Hairdresser %d is waiting for client...\n", hairdresser_id);
        fflush(stdout);
        lock_semaphore(sem_hairdressers);

        // JUST IN CASE HAIRDRESSER WAKES UP AND QUEUE IS EMPTY
        // It is not necessary, but if it happens it is better to handle it
        if (strlen(shared) == 0){
            printf("Hairdresser %d is going to sleep because he noticed that queue is empty\n", hairdresser_id);
            fflush(stdout);

            usleep(WAITING_TIME);

            printf("Hairdresser woke up\n");
            fflush(stdout);
            if (strlen(shared) == 0){
                printf("Hairdresser %d is going home \n", hairdresser_id);
                fflush(stdout);
                break;
            }
            continue;
        }
        // End of just in case
        
        lock_semaphore(sem_chairs);
        printf("Hairdresser is inviting client to chair\n");
        fflush(stdout);

        char haircut_type = get_from_queue(shared);
        set_hair_time(haircut_type);
        printf("Hairdresser is cutting hair of client with haircut type: %c\n", haircut_type);
        fflush(stdout);

        usleep(cut_time);

        printf("Hairdresser finished cutting hair of client with haircut type: %c\n", haircut_type);
        fflush(stdout);

        unlock_semaphore(sem_chairs);
        unlock_semaphore(sem_queue);
        
        if (strlen(shared) == 0){
            printf("Hairdresser is going to sleep\n");
            fflush(stdout);

            usleep(WAITING_TIME);

            printf("Hairdresser woke up\n");
            fflush(stdout);
            if (strlen(shared) == 0){
                printf("Hairdresser %d is going home\n", hairdresser_id);
                fflush(stdout);
                break;
            }
        }
    }
    // Others will wake up if hairdresser stuck in queue
    set_semaphore_value(sem_hairdressers, 1);
    // Unlinking shared memory
    shmdt(shared);

    return 0;
}

void get_semaphores(){
    sem_queue = open_semaphore(QUEUE_NAME);
    sem_chairs = open_semaphore(CHAIRS_NAME);
    sem_hairdressers = open_semaphore(HAIRDRESSERS_NAME);
}
void set_hair_time(char haircut_type){
    switch (haircut_type)
    {
        case 'A':
            cut_time = A;
            break;
        case 'B':
            cut_time = B;
            break;
        case 'C':
            cut_time = C;
            break;
        case 'D':
            cut_time = D;
            break;
        case 'E':
            cut_time = E;
            break;
        default:
            cut_time = CUT_TIME;
            break;
    }
}
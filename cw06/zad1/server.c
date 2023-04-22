#include "shared.h"


int connected_clients[MAX_CLIENTS] = {0};
char *filepath = "report.txt";
int server_queue;

void exit_handler(){
    msg_buffer message;
    message.m_type = STOP;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] == 0)
            continue;
        else if (msgsnd(connected_clients[i], &message, MSG_SIZE, 0) == -1)
            fprintf(stderr, "Cannot send STOP message to client with %d id.\n", i);
        else{
            connected_clients[i] = 0;
            printf("STOP message successfully sent to client with %d id.\n", i);
        }
    }
    printf("Server is shutting down.\n");
     if (msgctl(server_queue, IPC_RMID, NULL) == -1) {
        perror("ERROR! An error occurred while removing client queue.\n");
        exit(1);
    }
    printf("Server queue was successfully removed.\n");
}

void SIGINT_handler(int sig_no) {
    printf("\nReceived %d signal. Shutting down the server.\n", sig_no);
    exit(0);
}
void list_action(msg_buffer message) {
    char *text;
    char buffer[32];
    text = calloc(LINE_MAX,sizeof(char));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] != 0) {
            printf("Client %d is connected.\n", i); 
            sprintf(buffer, "%d ", i);
            strcat(text, buffer);
        }
    }
    msg_buffer new_message;
    new_message.m_type = LIST;
    strcpy(new_message.m_text, text);
    if (msgsnd(connected_clients[message.sender_id], &new_message, MSG_SIZE, 0) == -1) {
        perror("ERROR! An error occurred while sending a message from server.\n");
        exit(1);
    }
    printf("List successfully displayed.\n");
    free(text);
}

void init_client(msg_buffer message) {
    int new_id, found_free_id;
    key_t new_client_key;
    found_free_id = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] == 0) {
            found_free_id = 1;
            new_id = i;
            break;
        }
    }
    if (found_free_id == 0) {
        printf("ERROR! Cannot connect new client. Server is full.\n");
        return;
    }
    new_client_key = (key_t) strtol(message.m_text, NULL, 10);
    if ((connected_clients[new_id] = msgget(new_client_key, 0)) == -1) {
        fprintf(stderr, "ERROR! An error occurred: cannot get client queue identifier.\n");
    }
    msg_buffer new_message;
    new_message.m_type = INIT;
    new_message.sender_id = new_id;
    sprintf(new_message.m_text, "%d", new_id);

    if (msgsnd(connected_clients[new_id], &new_message, MSG_SIZE, 0) == -1) {
        perror("ERROR! An error occurred while sending message from server.\n");
        exit(1);
    }
    printf("Client with %d id has just joined the server.\n", new_id);
}

void to_one_action(int sender_id, int receiver_id, char *message) {
    if (connected_clients[receiver_id] == 0) {
        fprintf(stderr, "ERROR! An error occurred: There is no client with %d. Cannot send a message.\n", receiver_id);
        exit(1);
    }
    msg_buffer new_message;
    new_message.m_type = _2ONE;
    new_message.sender_id = sender_id;
    new_message.receiver_id = receiver_id;
    new_message.send_time = time(0);
    strcpy(new_message.m_text, message);
    if (msgsnd(connected_clients[receiver_id], &new_message, MSG_SIZE, 0) == -1) {
        perror("ERROR! An error occurred while sending a message to client.\n");
        exit(1);
    }
    printf("Message successfully sent to client with %d id.\n", receiver_id);
}
void to_all_action(msg_buffer message) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] && i != message.sender_id)
            to_one_action(message.sender_id, i, message.m_text);
    }
    printf("Message sent to all other clients by client with %d id.\n", message.sender_id);
}
int save_message(msg_buffer message) {
    FILE *file;
    if ((file = fopen(filepath, "a")) == NULL)
        return -1;
    time_t raw_time;
    struct tm;
    int sender_id;
    char *sender_message;

    time(&raw_time);
   

    sender_id = message.sender_id;
    sender_message = message.m_text;

    fprintf(file,"Date: %s \nMessage send by sender with %d id.\nText of the message: %s\n",ctime(&raw_time) ,sender_id, sender_message);

    fclose(file);
    return 0;
}
void handle_message(msg_buffer message) {
    if (save_message(message) == -1) {
        perror("ERROR! An error occurred while saving a message to a file.\n");
        exit(1);
    }
    switch (message.m_type) {
        case INIT:
            printf("Received INIT message from client %d.\n", message.sender_id);
            init_client(message);
            break;
        case LIST:
            printf("Received LIST message from client %d.\n", message.sender_id);
            list_action(message);
            break;
        case _2ONE:
            printf("Received 2ONE message from client %d.\n", message.sender_id);
            to_one_action(message.sender_id, message.receiver_id, message.m_text);
            break;
        case _2ALL:
            printf("Received 2ALL message from client %d.\n", message.sender_id);
            to_all_action(message);
            break;
        case STOP:
            printf("Received STOP message from client %d.\n", message.sender_id);

            connected_clients[message.sender_id] = 0;
            printf("Client %d has left the server.\n", message.sender_id);

            break;
        default:
            printf("Received unknown message from client %d.\n", message.sender_id);
            break;
    }

}


int main(int argc, char*argv[]){
    if (atexit(exit_handler) == -1) {
        perror("ERROR! Atexit error.\n");
        exit(1);
    }

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = SIGINT_handler;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("ERROR! Sigaction error.\n");
        exit(1);
    }

    char *home_path;
    key_t key;

    if ((home_path = getenv("HOME")) == NULL) {
        perror("ERROR! Cannot get HOME path.\n");
        exit(1);
    }

    if ((key = ftok(home_path, SERVER_KEY)) == -1) {
        perror("ERROR! Cannot get key.\n");
        exit(1);
    }

    if ((server_queue = msgget(key, IPC_CREAT | 0666)) == -1) {
        perror("ERROR! Cannot get server queue.\n");
        exit(1);
    }
    printf("Server is ready! Server queue started with %d key.\n", key);
    msg_buffer message;
    while (true) {
        printf("Waiting for message...\n");
        if (msgrcv(server_queue, &message, MSG_SIZE, -(INIT+1), 0) == -1) {
            perror("ERROR! Cannot receive message.\n");
            exit(1);
        }
        handle_message(message);
    }

}
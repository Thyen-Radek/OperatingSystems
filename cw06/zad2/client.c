#include "shared.h"

int server_queue;
int client_queue_desc;
int client_id, client_key, client_queue;
pid_t child_pid;
pid_t parent_pid;

int get_queue_name(char *queue_name) {
    char *home_path;
    if ((home_path = getenv("HOME")) == NULL) {
        perror("ERROR! An error occurred while setting home path.\n");
        exit(1);
    }
    char home_buffer[MAX_MESSAGE_LENGTH];
    strcpy(home_buffer, home_path);

    char *result;
    if (strtok_r(home_buffer, "/", &result) == NULL) {
        perror("ERROR! An error occurred: There are no more tokens.\n");
        return -1;
    }
    sprintf(queue_name, "/%s%d", result, getpid());
    return 0;
}


void exit_handler() {
    if (getpid() == parent_pid) {
        if (mq_close(server_queue) == -1) {
            perror("ERROR! An error occurred while closing the server queue.\n");
            exit(1);
        }
        if (mq_close(client_queue_desc) == -1) {
            perror("ERROR! An error occurred while closing the client queue.\n");
            exit(1);
        }
        char queue_name[MAX_MESSAGE_LENGTH];
        if (get_queue_name(queue_name) == -1) {
            perror("ERROR! An error occurred while getting queue name.\n");
            exit(1);
        }

        if (mq_unlink(queue_name) != 0) {
            perror("ERROR! An error occurred: cannot unlink client queue.\n");
            exit(1);
        }
        exit(0);
    }
}
void stop_action() {
    if (getpid() != parent_pid)
        kill(parent_pid, SIGINT);
    else {
        msg_buffer message;
        message.m_type = STOP;
        message.sender_id = client_id;
        if (child_pid > 0)
            kill(child_pid, SIGKILL);
        if (msg_send(server_queue, message, STOP) == -1) {
            perror("ERROR! An error occurred: cannot send a message to the server.\n");
        } else {
            printf("Client was successfully stopped.\n");
        }
        exit(1);
    }
}

msg_buffer *msg_receive(int type) {
    char message[MAX_MESSAGE_LENGTH];
    unsigned int u_type = (unsigned int) type;
    if (mq_receive(client_queue_desc, message, MAX_MESSAGE_LENGTH, type < 0 ? NULL : &u_type) == -1) {
        perror("ERROR! An error occurred: cannot get a message from the server queue.\n");
        return NULL;
    }
    return parse_message(message);
}


void list_action() {
    printf("Sending list request to the server.\n");
    msg_buffer message;
    message.m_type = LIST;
    message.sender_id = client_id;
    if (msg_send(server_queue, message, LIST)) {
        perror("ERROR! An error occurred: cannot send a message to all users.\n");
        exit(1);
    }
    msg_buffer *new_message;
    if (!(new_message = msg_receive(LIST)))
        return;

    printf("List of active clients: \n%s \n", new_message->m_text);
    free(new_message);

}
char *get_message_body(char *args) {
    char *text;
    if ((text = strtok(args, "\n\0")) == NULL) {
        fprintf(stderr, "ERROR! An error occurred: cannot get a message.\n");
        return NULL;
    }
    return text;
}
void to_one_action(int receiver_id, char *message) {
    printf("Sending ONE message to client with %d id.\n", receiver_id);
    msg_buffer new_message;
    new_message.m_type = _2ONE;
    new_message.sender_id = client_id;
    new_message.receiver_id = receiver_id;
    new_message.send_time = time(NULL);
    strcpy(new_message.m_text, message);
    if (msg_send(server_queue, new_message, _2ONE) == -1) {
        perror("ERROR! An error occurred: cannot send a message to all users.\n");
        exit(1);
    }
}
void to_all_action(char *message) {
    printf("Sending ALL message to all clients.\n");
    msg_buffer new_message;
    new_message.m_type = _2ALL;
    new_message.sender_id = client_id;
    strcpy(new_message.m_text, message);
    if (msg_send(server_queue, new_message, _2ALL) == -1) {
        perror("ERROR! An error occurred: cannot send a message to all users.\n");
        exit(1);
    }
}
int get_receiver_id(char *args) {
    char *str_receiver_id;
    char *new_args = "";
    if ((str_receiver_id = strtok_r(args, " \0", &new_args)) == NULL) {
        fprintf(stderr, "ERROR! An error occurred: cannot get receiver id.\n");
        return -1;
    }
    int receiver_id;
    if ((receiver_id = (int) strtol(str_receiver_id, NULL, 10)) < 0 || errno) {
        fprintf(stderr, "ERROR! An error occurred: invalid receiver id.\n");
        return -1;
    }
    strcpy(args, new_args);
    return receiver_id;
}

void handle_input() {
    char *command, *args;
    char input[LINE_MAX];
    printf("> ");

    if ((fgets(input, sizeof(input), stdin) == NULL)) {
        perror("ERROR! An error occurred while entering the command.\n");
        exit(1);
    }
    command = strtok_r(input, " ", &args);
    if (strncmp(command, "LIST", 4) == 0) {
        list_action();
    } 
    else if (strncmp(command, "2ALL", 4) == 0) {
        char *text;
        if ((text = get_message_body(args)) == NULL)
            stop_action();
        to_all_action(args);
    } else if (strncmp(command, "2ONE", 4) == 0) {
        int receiver_id;
        char *body = get_message_body(args);
        if ((receiver_id = get_receiver_id(args)) < 0 || !(body))
            stop_action();
        to_one_action(receiver_id, body);
    } else if (strncmp(command, "STOP", 4) == 0) {
        stop_action();
    } else
        printf("ERROR! An error occurred: command not found.\n");
}
void print_message(msg_buffer *message) {
    time_t send_time;
    struct tm;
    int sender_id;
    char *sender_message;

    send_time = message->send_time;

    sender_id = message->sender_id;
    sender_message = message->m_text;
    printf("Date: %s \nMessage send by sender with %d id.\nText of the message: %s\n",ctime(&send_time) ,sender_id, sender_message);
}

void handle_queue_message() {
    msg_buffer *message;
    if (!(message = msg_receive(-1)))
        return;
    switch (message->m_type) {
        case _2ONE:
            printf("Got 2ONE message.\n");
            print_message(message);
            break;
        case _2ALL:
            printf("Got 2ALL message.\n");
            print_message(message);
            break;
        case STOP:
            printf("Got STOP message.\n");
            stop_action();
            break;
        default:
            fprintf(stderr, "ERROR! An error occurred: there is no such type of message.\n");
    }
    free(message);
}
void init_client(){
    char queue_name[MAX_MESSAGE_LENGTH];
    if (get_queue_name(queue_name) == -1) {
        perror("ERROR! An error occurred while getting queue name.\n");
        exit(1);
    }
    printf("Client queue successfully created with %s name.\n", queue_name);
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_CLIENTS_MESSAGES;
    attr.mq_msgsize = MAX_MESSAGE_LENGTH;
    if ((client_queue_desc = mq_open(queue_name, O_RDONLY | O_CREAT, 0666, &attr)) == -1) {
        perror("ERROR! An error occurred while creating a queue.\n");
    }
    if ((server_queue = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        perror("ERROR! An error occurred while opening the server queue.\n");
        exit(1);
    }

    char init_name[MAX_MESSAGE_LENGTH];
    if (get_queue_name(init_name) == -1) {
        perror("ERROR! An error occurred while getting queue name.\n");
        exit(1);
    }
    msg_buffer message;
    message.m_type = INIT;
    sprintf(message.m_text, "%s", init_name);
    if (msg_send(server_queue, message, INIT) == -1) {
        perror("ERROR! An error occurred: cannot send a message to the server.\n");
        exit(1);
    }

    msg_buffer *new_message;
    if (!(new_message = msg_receive(INIT)))
        return;
    if ((client_id = (int) strtol(new_message->m_text, NULL, 10)) == 0 && errno) {
        perror("ERROR! An error occurred: cannot convert message text into a number.\n");
        free(new_message);
        exit(1);
    }
    free(new_message);
}
int main(int argc, char *argv[]){
    parent_pid = getpid();
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    if (atexit (exit_handler) != 0) {
        perror("ERROR! Cannot set exit handler.\n");
        exit(1);
    }

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = stop_action;
    action.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &action, NULL) != 0) {
        perror("ERROR! Sigaction error.\n");
        exit(1);
    }

    init_client();
    printf("Client with %d id has just joined the server.\n", client_id);

    struct mq_attr attr;
    if ((child_pid = fork()) == 0) {
        while (true) {
            handle_input();
        }
    } else {
        while (true) {
            if (mq_getattr(client_queue_desc, &attr) == -1) {
                perror("An error occurred: cannot get information about the client.\n");
                exit(1);
            }
            if (attr.mq_curmsgs > 0) {
                handle_queue_message();
            }
        }
    }
}
#include "shared.h"

int _;

int connect_unix(char* path) {
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, path, sizeof addr.sun_path -1);

  int sock = safe (socket(AF_UNIX, SOCK_STREAM, 0));
  safe (connect(sock, (struct sockaddr*) &addr, sizeof addr));

  return sock;
}

int connect_web(char* ipv4, int port) {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ipv4, &addr.sin_addr) <= 0) {
    puts("Invalid address\n");
    exit(0);
  }

  int sock = safe (socket(AF_INET, SOCK_STREAM, 0));
  safe (connect(sock, (struct sockaddr*) &addr, sizeof addr));
   
  return sock;
}

int sock;
void on_SIGINT(int _) {
  message msg = { .type = msg_disconnect };
  _ = write(sock, &msg, sizeof msg);
  exit(0);
}

int main(int argc, char** argv) {
  if (strcmp(argv[2], "web") == 0 && argc == 5) sock = connect_web(argv[3], atoi(argv[4]));
  else if (strcmp(argv[2], "unix") == 0 && argc == 4) sock = connect_unix(argv[3]);
  else {
    printf("Try [nick] [web|unix] [ip port|path]");
    printf("Example: ./client kamil web 127.0.0.1 8080");
    printf("Example2: ./client kamil unix socket");
    exit(0);
  }

  signal(SIGINT, on_SIGINT);
  char* nickname = argv[1];
  _ = write(sock, nickname, strlen(nickname));
  
  int epoll_fd = safe (epoll_create1(0));
  
  struct epoll_event stdin_event = { 
    .events = EPOLLIN | EPOLLPRI,
    .data = { .fd = STDIN_FILENO }
  };
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_event);

  struct epoll_event socket_event = { 
    .events = EPOLLIN | EPOLLPRI | EPOLLHUP,
    .data = { .fd = sock }
  };
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &socket_event);

  struct epoll_event events[2];
  loop {
    int nread = safe (epoll_wait(epoll_fd, events, 2, 1));
    repeat(nread) {
      if (events[i].data.fd == STDIN_FILENO) {

        char buffer[512] = {};

        int x = read(STDIN_FILENO, &buffer, 512);
        buffer[x] = 0;

        char korektor[] = " \t\n";
        char *token;
        int idx = 0;
        token = strtok( buffer, korektor );

        message_type type = -1;
        char other_nickname[MSG_LEN] = {};
        char text[MSG_LEN] = {};

        bool broke = false;

        if (token == NULL)
          continue;

        while( token != NULL ) {
            switch (idx) {
              case 0:
                if (strcmp(token, "LIST") == 0) type = msg_list;
                else if (strcmp(token, "2ALL") == 0) type = msg_tall;
                else if (strcmp(token, "2ONE") == 0) type = msg_tone;
                else if (strcmp(token, "STOP") == 0) type = msg_stop;
                else {
                  broke = true;
                  printf("Invalid command!\n");
                  printf("Try LIST, 2ALL, 2ONE, STOP\n");
                  type = -1;
                }
                break;
              case 1:
                memcpy(text, token, strlen(token)*sizeof(char));
                text[strlen(token)] = 0;
                break;
              case 2:
                memcpy(other_nickname, token, strlen(token)*sizeof(char));
                other_nickname[strlen(token)] = 0;
                break;
              case 3:
                broke = true;
                break;
            }

            if (broke)
              break;

            idx++;
            token = strtok( NULL, korektor );
        }
        if (broke)
          continue;

        message msg;
        msg.type = type;
        memcpy(&msg.other_nickname, other_nickname, strlen(other_nickname)+1);
        memcpy(&msg.text, text, strlen(text)+1);

        _ = write(sock, &msg, sizeof msg);

      } else { 

        message msg;
        _ = read(sock, &msg, sizeof msg);

        if (msg.type == msg_username_taken) {
          puts("This username is already taken\n");
          close(sock);
          exit(0);
        } else if (msg.type == msg_server_full) {
          puts("Server is full\n");
          close(sock);
          exit(0);
        } else if (events[i].events & EPOLLHUP) {
          puts("Disconnected\n");
          exit(0);
        } else if (msg.type == msg_ping) {
          _ = write(sock, &msg, sizeof msg);
        } else if (msg.type == msg_stop) {
          puts("Stopping\n");
          close(sock);
          exit(0);
        } else if (msg.type == msg_get) {
          puts(msg.text);
        }
      }
    }
  }
}
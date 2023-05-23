#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/epoll.h>

#include <time.h> 
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#define MSG_LEN 256

typedef enum {
    msg_ping,
    msg_username_taken,
    msg_server_full,
    msg_disconnect,
    msg_get,
    msg_init,
    msg_list,
    msg_tone,
    msg_tall,
    msg_stop,
    msg_connect,
  } message_type;

typedef struct {
  message_type type;
  char text[MSG_LEN]; // INCLUDES NICKNAME
  char nickname[MSG_LEN];
  char other_nickname[MSG_LEN];
} message;

#define safe(expr)                                                              \
  ({                                                                            \
    typeof(expr) __tmp = expr;                                                  \
    if (__tmp == -1) {                                                          \
      printf("%s:%d "#expr" failed: %s\n", __FILE__, __LINE__, strerror(errno));\
      exit(EXIT_FAILURE);                                                       \
    }                                                                           \
    __tmp;                                                                      \
  })
#define loop for(;;)
#define find(init, cond) ({ int index = -1;  for (init) if (cond) { index = i; break; } index; })
#define repeat(n) for(int i = 0; i < n; i++)
#define print(x) write(STDOUT_FILENO, x, sizeof(x))

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define STDIN_FILENO 0

static int portnum = 0;
static char* ipaddr;

int main(int argc, char* argv[]) {
  /* definitions */
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;
  char message_buf[MAX];

  /* args parsing */
  if (argc >= 2) { ipaddr = !strcmp(argv[1], "localhost") ? "127.0.0.1" : argv[1];
  } else ipaddr = "127.0.0.1";

  if (argc >= 3) { portnum = atoi(argv[2]);
  } else portnum = 6600;

  bzero(message_buf, sizeof(message_buf));
  char c;
  for (int i = 0; ((c = getchar()) != EOF); i++) {
    message_buf[i] = c;
  }

  if (argc >= 4) {
    bzero(message_buf, sizeof(message_buf));
    strcpy(message_buf, argv[3]);
  }

  /* start udp connection */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (!sockfd) { printf("socket creation failed!\n"); exit(0);
  } /* else printf("socket successfully created!\n"); */

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(ipaddr);
  servaddr.sin_port = htons(portnum);

  if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
    printf("connection to server failed!\n"); exit(0);
  } /* else printf("connected to server!\n"); */

  /* messaging */
  write(sockfd, message_buf, sizeof(message_buf));
  bzero(message_buf, sizeof(message_buf));
  read(sockfd, message_buf, sizeof(message_buf));

  if (!(strncmp(message_buf, "ACK", 3))) {
    char error[MAX - 3];
    strncpy(error, message_buf + 3, MAX - 3);
    printf("server error:%s", error);
  } else if (!(strncmp(message_buf, "OK", 2))) {
    char message[MAX - 2];
    strncpy(message, message_buf + 2, MAX - 2);
    printf("server returned:%s", message);
  } else printf("unknown error");

  close(sockfd);
}


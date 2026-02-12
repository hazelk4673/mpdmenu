
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80

static int portnum = 0;
static char* ipaddr;

int main(int argc, char* argv[]) {
  /* definitions */
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;
  char message_buf[MAX] = "empty message\n";

  /* args parsing */
  if (argc >= 2) { portnum = atoi(argv[1]);
  } else portnum = 6600;

  if (argc >= 3) { ipaddr = !strcmp(argv[2], "localhost") ? "127.0.0.1" : argv[2];
  } else ipaddr = "127.0.0.1";

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
  }

  close(sockfd);
}


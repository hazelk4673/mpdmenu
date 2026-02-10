#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int connfd) {
  char buf[MAX];
  int n;

  for (;;) {
    bzero(buf, MAX);

    read(connfd, buf, sizeof(buf));

    printf("from client : %s\n To client : ", buf);
    bzero(buf, MAX);
    n = 0;

    while ((buf[n++] = getchar()) != '\n');

    write(connfd, buf, sizeof(buf));

    if (strncmp("exit", buf, 4) == 0) {
      printf("server exit...\n");
      break;
    }
  }
}

int main() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (!sockfd) {
    printf("socket creation failed!\n");
    exit(1);
  } else { printf("socket successfully created!\n"); }

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    printf("socket bind failed!\n");
    exit(0);
  } else { printf("socket successfully binded!\n"); }

  if ((listen(sockfd, 5)) != 0) {
    printf("listen failed!\n");
    exit(0);
  } else { printf("server listening...\n"); }

  len = sizeof(cli);

  connfd = accept(sockfd, (SA*)&cli, &len);
  if (connfd < 0) {
    printf("server accept failed!\n");
    exit(0);
  } else { printf("server accepted client!\n"); }

  func(connfd);

  close(sockfd);
}
  

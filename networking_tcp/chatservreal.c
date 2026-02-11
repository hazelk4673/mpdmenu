#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MESSAGE 80
#define MAX_BACKLOG 10
#define SA struct sockaddr

int main(int argc, char* argv[]) {

  int sockfd, connfd, len, port_num, flags;
  struct sockaddr_in servaddr, cli;

  port_num = (argc >= 2) ? atoi(argv[1]) : 8080;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) != 0) { 
    printf("socket creation failed!\n"); exit(1); 
  } else printf("socket successfully created!\n");

  if ((flags = fcntl(sockfd, F_GETFL, 0)) == -1) {
    printf("error getting socket flags!\n");
  } else printf("got socket flags!\n");

  if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) != 0) {
    printf("error setting socket flags!\n");
  } else printf("set socket flags!\n");

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port_num);

  if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
    printf("socket bind failed!\n"); exit(0); 
  } else printf("socket successfully binded!");

    if ((listen(sockfd, MAX_BACKLOG)) != 0) {
      printf("listen failed!\n"); exit(0); 
    } else printf("server listening...\n");

    /* 
     * idk exactly what im doing here but this is the point at which
     * i need to diverge in order to accept multiple clients
     */

    len = sizeof(cli);

    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) { printf("server accept failed!\n"); exit(0); 
    } else printf("server accepted client!\n");






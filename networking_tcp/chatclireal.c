#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_MESSAGE 80
#define SA struct sockaddr

void chat(int sockfd) {

  char buf[MAX_MESSAGE];
  int n;

  for (;;) {
    bzero(buf, sizeof(buf));
    printf(" > ");
    n = 0;

    while ((buf[n++] = getchar()) != '\n');

    write (sockfd, buf, sizeof(buf));
    bzero(buf, sizeof(buf));
    read(sockfd, buf, sizeof(buf));

    /*
     * protocolslop, basically if server returns ACK
     * it will return an error afterwards
     */
    if (strncmp(buf, "ACK", 3) == 0) {
      /*
       * this might be some messed up evil pointer 
       * magic, basically I just want the buffer from the 4th elem onwards 
       */
      printf("server error: %s" (buf * (sizeof(char) * 3)));
    } else if (strncmp(buf, "OK", 2) != 0) { 
      printf("server connection lost\n"); 
    }
  }
}

int main(int argc, char* argv[]) {

  int sockfd, connfd, port_num;
  struct sockaddr_in servaddr, cli;
  char server_address[16];

  server_address = (argc >= 2) ? argv[1] : "127.0.0.1";
  port_num = (argc >= 3) ? atoi(argv[2]) : 8080;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (!sockfd) { printf("socket creation failed!\n"); exit(-1); }
  else printf("socket successfully created!\n");

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(server_address);
  servaddr.sin_port = htons(port_num);

  if (connetc(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
    printf("connection to server failed!\n");
    exit(0);
  } else printf("connected to server!\n");

  chat(sockfd);

  close(sockfd);
}


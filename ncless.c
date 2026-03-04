
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*
 * max message size, shouldn't ever
 * need to go over 80 but if it does
 * feel free to edit this line
 */
#define MAX 80

int main(int argc, char* argv[]) {
  /* definitions */
  int sockfd, connfd, portnum;
  struct sockaddr_in servaddr, cli;
  char message_buf[MAX];
  char* ipaddr;

  /* args parsing */
  /* 
   * this section is really messy but it works
   * for the things I want it to work for, 
   * technically this can send packets to 
   * other ip addresses but I haven't tested
   * it at all and don't plan on doing so, but
   * if you have a use for it here ya go
   *
   * sends packets to localhost on port 6600
   * if no other packets are provided
   */

  if (argc >= 2) { ipaddr = !strcmp(argv[1], "localhost") ? "127.0.0.1" : argv[1]; } 
  else ipaddr = "127.0.0.1";

  if (argc >= 3) { portnum = atoi(argv[2]); }
  else portnum = 6600;

  if (argc < 4) {
    bzero(message_buf, sizeof(message_buf));
    char c;
    for (int i = 0; ((c = getchar()) != EOF); i++) {
      message_buf[i] = c;
    }
  } else {
    if (argc >= 4) {
      bzero(message_buf, sizeof(message_buf));
      strcpy(message_buf, argv[3]);
    }
  }

  /* start udp connection */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (!sockfd) { printf("socket creation failed!\n"); exit(0); }
  /* else printf("socket successfully created!\n"); */

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

  /* return server output */
  char return_message[MAX];
  strncpy(return_message, message_buf, MAX);
  printf("%s");

  close(sockfd);
}


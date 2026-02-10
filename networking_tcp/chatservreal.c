#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define SA struct sockaddr

int main(int argc, char* argv[]) {

  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (!sockfd) { printf("socket creation failed!\n"); exit(1); }
  else printf("socket successfully created!\n");


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "./a.out ipaddr port\n");
    exit(1);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  //char addr[17];
  //strcpy(addr, argv[1]);
  //int port = atoi(argv[2]);

  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(serveraddr));
  socklen_t addrlen = sizeof(serveraddr);

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);//addr);
  serveraddr.sin_port = htons(INADDR_ANY);//atoi(argv[2]));//port);

  if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    perror("bind");
    exit(1);
  }

  if (listen(sockfd, 20) < 0)
  {
    perror("listen");
    exit(1);
  }

  int cfd;
  struct sockaddr_in clientaddr;
  memset(&clientaddr, 0, sizeof(clientaddr));

  if ((cfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
  {
    perror("accept");
    exit(1);
  }

  printf("[ %s - %d ]# \n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

  char buf[128] = "";

  if (recv(cfd, buf, sizeof(buf), 0) < 0)
  {
    perror("recv");
    exit(1);
  }

  printf("buf = %s\n", buf);

  strcat(buf, " ok");
  if (send(cfd, buf, sizeof(buf), 0) < 0)
  {
    perror("recv");
    exit(1);
  }

  close(cfd);
  close(sockfd);

  return 0;
}

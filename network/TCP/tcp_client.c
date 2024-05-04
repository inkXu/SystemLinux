/*
 * author: inkXu
 * date:   2022.9.23
 *
 * tcp客户端
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define M 128

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "./a.out ip_addr port\n");
    exit(1);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in serveraddr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  memset(&serveraddr, 0, addrlen);

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(htons(atoi(argv[2])));

  if (connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    perror("connnect");
    exit(1);
  }

  char text[M] = "";
  fgets(text, M, stdin);

  if (send(sockfd, text, M, 0) < 0)
  {
    perror("send");
    exit(1);
  }

  if (recv(sockfd, text, M, 0) < 0)
  {
    perror("recv");
    exit(1);
  }

  printf("text = %s\n", text);

  close(sockfd);

  return 0;
}

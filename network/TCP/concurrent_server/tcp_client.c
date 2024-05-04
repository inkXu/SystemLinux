#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define M 1024
#define MSG_LOG(errmsg) do {\
                          perror(errmsg);\
                          exit(1);\
                        }while(0);

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "%s ip_addr port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    MSG_LOG("socket");
  }

  struct sockaddr_in serveraddr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(atoi(argv[2]));

  if (connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) != 0)
  {
    MSG_LOG("connect");
  }

  char text[M];
  char buf[M];
  ssize_t bytes;

  while(1)
  {
    bzero(text, M);

    fgets(text, M, stdin);
    text[strlen(text) -1] = '\0';
    if (send(sockfd, text, M, 0) < 0)
    {
      MSG_LOG("send");
    }

    bzero(buf, M);
    if ((bytes = recv(sockfd, buf, M, 0)) < 0)
    {
      MSG_LOG("recv");
    }
    else if (bytes == 0)
    {
      printf("Connection Break!\n");
      exit(EXIT_FAILURE);
    }
    else
    {
      printf("[%s - %d]# ", inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));
      printf("%s\n", buf);
    }
  }

  close(sockfd);

  return 0;
}

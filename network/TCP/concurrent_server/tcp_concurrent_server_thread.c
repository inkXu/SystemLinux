/*
 * author: inkXu
 * date:   2022.10.2
 *
 * 使用多线程实现tcp并发服务器
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define M 1024
#define ERR_LOG(errmsg) do {\
                         perror(errmsg);\
                         exit(1);\
                        }while(0);

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "%s ip_addr port\n", argv[0]);
    exit(1);
  }

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    ERR_LOG("socket");
  }

  struct sockaddr_in serveraddr;
  socklen_t addrlen = sizeof(serveraddr);
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(atoi(argv[2]));

  if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    ERR_LOG("bind");
  }

  if (listen(sockfd, 20) < 0)
  {
    ERR_LOG("listen");
  }

  while(1)
  {
    int fd;
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, addrlen);

    if ((fd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
    {
      ERR_LOG("accept");
    }

    printf("%s:%d is Connected!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    pthread_t thread;
    void *pfunc(void *sockfd);

    if (pthread_create(&thread, NULL, pfunc, (void *)&fd) != 0)
    {
      ERR_LOG("pthread_create");
    }

    // 设置为分离态，自动回收子线程资源
    pthread_detach(thread);
  }

  close(sockfd);

  return 0;
}

void *pfunc(void *sockfd)
{
  int fd = *(int *)sockfd;
  char buf[M];
  ssize_t bytes;
  while(1)
  {
    bzero(buf, M);
    if ((bytes = recv(fd, buf, M, 0)) < 0)
    {
      ERR_LOG("recv");
    }
    else if (bytes == 0)
    {
      printf("Connection Break!\n");
      pthread_exit(0);
    }
    else
    {
      if (strncmp(buf, "quit", 4) == 0)
      {
        printf("Host is Quit!\n");
        close(fd);
        pthread_exit(0);
      }
    }

    printf("buf = %s\n", buf);

    strcat(buf, " ok");

    if (send(fd, buf, M, 0) < 0)
    {
      ERR_LOG("send");
    }
  }
}

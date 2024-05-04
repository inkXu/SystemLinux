/*
 * author: inkXu
 * date:   2022.10.2
 *
 * 使用多进程的方法实现tcp并发服务器
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define M 128

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "%s ip_addr port\n", argv[0]);
    exit(1);
  }

  // 创建套接字
  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  // 将套接字设置为端口复用
  int on = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    perror("setsockopt");
    exit(1);
  }

  // 服务器的网络信息结构体
  struct sockaddr_in serveraddr;
  socklen_t addrlen = sizeof(serveraddr);
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(atoi(argv[2]));

  // 将服务器与网络信息结构体绑定
  if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    perror("bind");
    exit(1);
  }

  // 将套接字设置为被动监听状态
  if (listen(sockfd, 20) < 0)
  {
    perror("listen");
    exit(1);
  }

  // 使用信号，异步方式回收僵尸进程，一般也是用该信号来回收僵尸进程
  void handler(int sig);
  // SIGCHLD信号在进程结束时会产生，以前signal.txt文件的常用信号有说明
  signal(SIGCHLD, handler);

  while(1)
  {
    // 阻塞等待客户端的连接请求
    int fd;
    struct sockaddr_in clientaddr;
    if ((fd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
    {
      perror("accept");
      exit(1);
    }

    // 打印客户端信息
    printf("%s:%d is connected\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    // 客户端连接成功后再创建子进程，这样父子进程才能共享与客户端连接的文件描述符，
    // 使用fork函数创建子进程，父进程继续负责连接，子进程负责与客户端进行通信
    pid_t pid;
    // 父进程负责执行accept，所以if语句结束后会阻塞在accept函数，父进程这里不要写exit或是wait
    // 若果使用wait回收僵尸进程，那么父进程会一直阻塞在wait，导致其它客户端不能与之通信
    // 必须使用fork创建子进程，vfork函数会先调度子进程，执行完才会调度父进程
    if ((pid = fork()) > 0)
    {
    }
    else if(pid == 0) //子进程负责和客户端进行通信
    {
      char buf[M];
      ssize_t bytes = 0;
      while(1)
      {
        bzero(buf, M);

        if ((bytes = recv(fd, buf, M, 0)) < 0)
        {
          perror("recv");
          exit(1);
        }
        else if (bytes == 0) //关闭文件描述符时会向客户端/服务器发送一个0字符的数据包
        {
          printf("Host %s is Disconnect!\n", inet_ntoa(clientaddr.sin_addr));
          exit(0);
        }
        else
        {
          if (strncmp(buf, "quit", 4) == 0)
          {
            printf("Host %s is Quit!\n", inet_ntoa(clientaddr.sin_addr));
            exit(0);
          }
        }

        printf("[%s - %d]# %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

        strcat(buf, " ok");

        if (send(fd, buf, M, 0) < 0)
        {
           perror("send");
           exit(1);
        }
      }
    }
    else
    {
      perror("vfork");
      exit(1);
    }
  }

  return 0;
}

// 信号处理方式
void handler(int sig)
{
  wait(NULL);
}


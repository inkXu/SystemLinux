/*
 * author: inkXu
 * date:   2022.9.10
 *
 * 套接字的创建
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{
  int socket(int domain, int type, int protocol);

  int sockfd;

  //创建一个网络编程使用的套接字，使用UDP协议，数据报套接字，不需要其它的协议
  if (( sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  printf("sockfd =  %d\n", sockfd);

  return 0;
}

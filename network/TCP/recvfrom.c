/*
 * author: inkXu
 * date:   2022.9.10
 *
 * ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
 *                         struct sockaddr *src_addr, socklen_t *addrlen);
 * 接收udp数据，并将源地址信息保存在src_addr指向的结构体
 * sockfd: 套接字
 * buf: 接收数据缓冲区
 * len: 接收数据缓冲区的大小
 * flags: 套接字标志（常为0）
 * src_addr: 源地址结构体指针，用来保存数据的来源
 * addrlen: src_addr所指内容长度
 * 成功返回接收到的字符数，失败返回-1
 * 注意：
 *     通过src_addr和addrlen参数存放数据来源信息src_addr和addrlen可以为NULL，表示不保存数据来源
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define N 128

int main(int argc, char const *argv[])
{
  if (argc < 3) {
    printf("./a.out ipaddr port\n");
    exit(EXIT_FAILURE);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  printf("sockfd = %d\n", sockfd);

  //填充网络信息结构体
  struct sockaddr_in sockserver;

  sockserver.sin_family = AF_INET;
  sockserver.sin_addr.s_addr = inet_addr(argv[1]);
  sockserver.sin_port = htons(atoi(argv[2]));

  if (bind(sockfd, (struct sockaddr *)&sockserver, sizeof(sockserver)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  char buf[N] = "";

  struct sockaddr_in sockclient;
  socklen_t socklen = sizeof(struct sockaddr_in);

  while(1) {
    memset(buf, 0, sizeof(buf));

    if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&sockclient, &socklen) == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }

    printf("src = %s, port = %d: ", inet_ntoa(sockclient.sin_addr), ntohs(sockclient.sin_port));
    printf("buf = %s\n", buf);
  }

  return 0;
}

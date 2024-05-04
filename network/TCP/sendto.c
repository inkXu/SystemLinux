/*
 * author: inkXu
 * date:   2022.9.10
 *
 * 发送数据
 *
 * ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
 *                       const struct sockaddr *dest_addr, socklen_t addrlen);
 * 功能：
 *     向dest_addr结构体指针中指定的ip发送UDP数据
 * 参数：
 *     sockfd：套接字
 *     buf：发送数据缓冲区
 *     len：发送数据缓冲区的大小
 *     flags：一般为0
 *     dest_addr：指向目的主机地址结构体的指针
 *     addrlen：dest_addr所指向内容的长度
 * 注意：
 *     通过dest_addr和addrlen确定目的地址
 *     可以发送长度为0的UDP数据报
 * 返回值：
 *     成功返回发送数据的字符数
 *     失败返回-1
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define N 128

int main(int argc, char const *argv[])
{
  if (argc < 3) {
    printf("./a.out ipaddr port\n");
    exit(EXIT_FAILURE);
  }

  //创建一个套接字
  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  printf("sockfd = %d\n", sockfd);

  //填充UDP网络结构体
  struct sockaddr_in sockserver;

  //使用ipv4协议
  sockserver.sin_family = AF_INET;
  //指定发送和接收的的端口为函数传入的第三个参数端口
  sockserver.sin_port = htons(atoi(argv[2]));
  //指定发送的服务器IP地址为argv[1]
  sockserver.sin_addr.s_addr = inet_addr(argv[1]);

  char buf[N] = "";

  //向指定的服务器循环发送信息
  while(1) {
    memset(buf, 0, sizeof(buf));
    fgets(buf, N, stdin);

    //为了使发送的数据不会自己换行，将数据后的\n改成\0
    //buf[strlen(buf) - 1] = '\0';

    if (sendto(sockfd, buf, strlen(buf)-1, 0, (struct sockaddr *)&sockserver, sizeof(sockserver)) == -1) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
  }

  //关闭网络套接字
  close(sockfd);

  return 0;
}

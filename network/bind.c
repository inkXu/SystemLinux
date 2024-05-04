/*
 * author: inkXu
 * date:   2022.9.12
 *
 * int bind(int socket, const struct sockaddr *address, socklen_t address_len);
 * 将本地协议地址（网络信息结构体）与sockfd绑定
 * socket: socket套接字
 * address: 指向特定协议的地址结构体，网络信息结构体
 * address_len: 该地址结构（网络信息结构体）的长度
 * 成功返回0，失败返回其它
 *
 * 由于服务器是被动的，客户端是主动的，所以一般先运行服务器，后运行客户端，所以服务器需要固定自己的信息(ip地
   址和端口号)，这样客户端才可以找到服务器并与之通信，而客户端一般不要bind绑定，因为系统会自动分配IP地址和端
   口号给客户主机，就像你的电脑手机，都是路由器或ap给你分配IP地址
 * udp网络程序想要收取数据需要确定IP地址、端口号，所以接收端（服务器）需要使用bind函数，完成地址结构与socket
   套接字的绑定，这样ip和port就固定了，发送端在sendto函数中指定接收端的ip、port就可以发送数据了
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(int argc, char const *argv[])
{
  //创建网络套接字
  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket");
    exit(1);
  }

  //创建网络结构体
  struct sockaddr_in sockaddr;

  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr("192.168.3.216");
  sockaddr.sin_port = htons(9798);

  //对服务器的端口号，IP地址进行绑定
  if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) != 0)
  {
    perror("bind");
    exit(1);
  }

  return 0;
}

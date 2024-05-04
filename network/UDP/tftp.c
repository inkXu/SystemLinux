/*
 * author: inkXu
 * date;   2022.9.13
 *
 * tftp download file
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define M 516

void do_download(int sockfd, struct sockaddr_in *serveraddr, char const *filename);

int main(int argc, char const *argv[])
{
  //程序的运行需要服务器IP地址和需要在服务器上下载的文件名
  if (argc < 3)
  {
    printf("./a.out ip filename\n");
    exit(EXIT_FAILURE);
  }

  int sockfd;
  struct sockaddr_in serveraddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(69);

  do_download(sockfd, &serveraddr, argv[2]);

  close(sockfd);

  return 0;
}

void do_download(int sockfd, struct sockaddr_in *serveraddr, char const *filename)
{
  char text[128] = "";             //客户端向tftp服务器发送下载请求的报文
  int text_len = 0;                //请求报文的长度
  socklen_t addrlen = sizeof(struct sockaddr_in);
  int flag = 0;                    //标识是否创建且打开本地文件
  char recvdata[M + 1] = "";           //从服务器上接收的数据
  int num = 0;                     //块编码
  int fd;                          //打开的本地文件的文件描述符
  ssize_t bytes = 0;               //接收数据大小

  //拼接读写请求的报文，需要获取sprintf的返回值，即拼接的长度，因为其中有\0字符，所以不能通过strlen函数
  //获取到拼接后text的长度，所以在此直接接收sprintf函数的返回值
  text_len = sprintf(text, "%c%c%s%c%s%c", 0, 1, filename, 0, "octet", 0);

  //向服务器发送向服务器读取文件的请求报文
  if (sendto(sockfd, text, text_len, 0, (struct sockaddr *)serveraddr, addrlen) < 0)
  {
    perror("sendto");
    printf("errno = %d\n", errno);
    exit(EXIT_FAILURE);
  }

  while(1)
  {
    memset(recvdata, 0, sizeof(recvdata));

    //接收服务器发送的数据包
    if (bytes = recvfrom(sockfd, recvdata, M, 0, (struct sockaddr *)serveraddr, &addrlen) < 0)
    {
      perror("recvfrom");
      return ;
    }

    //tftp协议规定数据包的前两个字节为操作码，操作码为5表示发生错误
    if(recvdata[1] == 5)
    {
      printf("error: %s\n", recvdata + 4);
      return ;
    }
    if(recvdata[1] == 3)
    {
      if (flag == 0)
      {
        if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0)
        {
          perror("open");
          exit(EXIT_FAILURE);
        }
        flag = 1;
      }

      if (num + 1 == ntohs(*(unsigned short *)(recvdata + 2)))
      {
        num = ntohs(*(unsigned short *)(recvdata + 2));
        if (write(fd, recvdata + 4, sizeof(recvdata) - 4) < 0)
        {
          perror("write");
          return ;
        }

        recvdata[1] = 4;

        //发送ack应答包
        if (sendto(sockfd, recvdata, 4, 0, (struct sockaddr *)serveraddr, addrlen) < 0)
        {
          perror("sendto");
          return ;
        }

        if (bytes < 516)
        {
          break;
        }
      }
    }
  }

  close(fd);
}

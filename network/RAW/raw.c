#include <stdio.h>        //printf
#include <stdlib.h>       //exit
#include <sys/types.h>
#include <sys/socket.h>   //socket
#include <arpa/inet.h>    //htons
#include <netinet/ether.h>    //ETH_P_ALL ETH_P_IP ETH_P_ARP

int main(int argc, char const *argv[])
{
  int sockfd;
  if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  printf("sockfd = %d\n", sockfd);

  // 不要sizeof(ETH_P_ALL)，测出的值是4字节，不是2个字节，可能是编译器的机制，直接测试
  // 一个十六进制数时，默认会创建一个int类型的变量来存储这个数，所以sizeof测的是4个字节
  printf("ETH_P_ALL = %#x\n", ETH_P_ALL);
  printf("ETH_P_ARP = %#x\n", ETH_P_ARP);
  printf("ETH_P_IP = %#x\n", ETH_P_IP);

  return 0;
}

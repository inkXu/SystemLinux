#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>

#define M 4096

int main(int argc, char const *argv[])
{
  int sockfd;
  if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  //注意使用的是无符号的字符串
  short eth_protocol[] = {0x0806, 0x8035, 0x0800};
  int net_protocol[] = {1, 2, 6, 17};
  char protocol_name[][5] = {"ARP", "RARP", "IP", "ICMP", "IGMP", "TCP", "UDP"};
  unsigned char head[M] = "";
  unsigned char src_mac[18] = "";
  unsigned char dst_mac[18] = "";
  unsigned short type;
  unsigned char src_ip[16] = "";
  unsigned char dst_ip[16] = "";

  while(1)
  {
    bzero(src_mac, 18);
    bzero(dst_mac, 18);
    if (recvfrom(sockfd, head, M, 0, NULL, NULL) < 0)
    {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    sprintf(src_mac, "%x:%x:%x:%x:%x:%x", head[0], head[1], head[2], head[3], head[4], head[5]);
    sprintf(dst_mac, "%x:%x:%x:%x:%x:%x", head[6], head[7], head[8], head[9], head[10], head[11]);
    printf("MAC: %s >>> %s\n", src_mac, dst_mac);

    type = ntohs(*(unsigned short *)(head + 12));

    if (type == eth_protocol[2])
    {
      printf("protocol type: %s\n", protocol_name[2]);
      sprintf(src_ip, "%d.%d.%d.%d", head[26], head[27], head[28], head[29]);
      sprintf(dst_ip, "%d.%d.%d.%d", head[30], head[31], head[32], head[33]);
      printf("IP: %s >>> %s\n", src_ip, dst_ip);
    }
    else if (type == eth_protocol[0])
    {
      printf("protocol type: %s\n", protocol_name[0]);
    }
    else if (type == eth_protocol[1])
    {
      printf("protocol type: %s\n", protocol_name[1]);
    }
  }

  close(sockfd);

  return 0;
}

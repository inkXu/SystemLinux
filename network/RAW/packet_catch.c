/*
 * author: inkXu
 * date:   2022.10.10
 *
 * 抓包程序
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ether.h>

#define M 4096
#define ERR_LOG(err_msg) do {\
				perror(err_msg);\
				exit(EXIT_FAILURE);\
			 } while(0);

int main(int argc, char const *argv[])
{
  int sockfd;
  if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    ERR_LOG("socket");
  }

  unsigned char data[M] = "";
  unsigned char src_mac[18] = "";
  unsigned char dst_mac[18] = "";
  unsigned short eth_type;
  short eth_protocol[] = {0x0800, 0x0806, 0x8035};
  int link_protocol[] = {1, 2, 6, 17};
  char protocol[][5] = {"IP", "ARP", "RARP", "ICMP", "IGMP", "TCP", "UDP"};

  while(1)
  {
    printf("*------------------------------------------------------*\n");
    bzero(data, M);
    if (recvfrom(sockfd, data, M, 0, NULL, NULL) < 0)
    {
      ERR_LOG("recv");
    }

    sprintf(src_mac, "%x:%x:%x:%x:%x:%x", data[0], data[1], data[2], data[3], data[4], data[5]);
    sprintf(dst_mac, "%x:%x:%x:%x:%x:%x", data[6], data[7], data[8], data[9], data[10], data[11]);
    printf("MAC: %s >>> %s\n", src_mac, dst_mac);

    eth_type = ntohs(*(unsigned short *)(data + 12));
    if (eth_type == eth_protocol[0])
    {
      unsigned short src_port, dst_port;
      unsigned char src_ip[16] = "";
      unsigned char dst_ip[16] = "";
      unsigned int link_layer;

      printf("ethernet protocol: %s\n", protocol[0]);

      sprintf(src_ip, "%d.%d.%d.%d", data[26], data[27], data[28], data[29]);
      sprintf(dst_ip, "%d.%d.%d.%d", data[30], data[31], data[32], data[33]);
      printf("IP: %s >>> %s\n", src_ip, dst_ip);

      link_layer = data[23];
      if (link_layer == link_protocol[0])
      {
        printf("link layer protocol: %s\n", protocol[3]);
      }
      else if (link_layer == link_protocol[1])
      {
        printf("link layer protocol: %s\n", protocol[4]);
      }
      else if (link_layer == link_protocol[2])
      {
        unsigned char ip_head_len;
        unsigned short ip_len;

        printf("link layer protocol: %s\n", protocol[5]);

        src_port = ntohs(*(unsigned short *)data + 34);
        dst_port = ntohs(*(unsigned short *)data + 36);
        printf("PORT: %d >>> %d\n", src_port, dst_port);

        ip_head_len = ((*(unsigned char *)(data + 14)) & 0x0f) * 4;
        ip_len = ntohs(*(unsigned short *)(data + 16));
        printf("ip head length: %d, ip length: %d\n", ip_head_len, ip_len);
      }
      else if (link_layer == link_protocol[3])
      {
        printf("link layer protocol: %s\n", protocol[6]);
        src_port = ntohs(*(unsigned short *)data + 34);
        dst_port = ntohs(*(unsigned short *)data + 36);
        printf("PORT: %d >>> %d\n", src_port, dst_port);
      }
    }
    else if (eth_type == eth_protocol[1])
    {
      printf("ethernet protocol: %s\n", protocol[1]);
    }
    else if (eth_type == eth_protocol[2])
    {
      unsigned short opcode;

      printf("ethernet protocol: %s\n", protocol[2]);

      opcode = ntohs(*(unsigned short *)(data + 20));
      printf("opcode: %#x\n", opcode);
    }
    printf("*------------------------------------------------------*\n\n\n");
    sleep(1);
  }

  close(sockfd);

  return 0;
}

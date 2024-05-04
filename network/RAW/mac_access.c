/*
 * author: inkXu
 * date:   2022.10.11
 *
 * MAC地址扫描器
 * 通过ARP协议获取目的主机MAC地址
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>

#define M (28 + 14)
#define ERR_LOG(err_msg) do{\
			     perror(err_msg);\
			     exit(EXIT_FAILURE);\
			 }while(0);

int main(int argc, char const *argv[])
{
  int sock_raw_fd;

  /*                        注意， 注意， 注意                       */
  /*           ETH_P_ALL占两个字节，需要转换为网络字节序!!!          */

  if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
  {
    ERR_LOG("socket");
  }

  unsigned char arp_packet[M] = {
    /*      ethernet       */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  //destination mac
    0x00, 0x0c, 0x29, 0x8d, 0xd9, 0x0e,  //source mac
    0x08, 0x06,                          //protocol type 0x0806 ARP

    /*      ARP       */
    0x00, 0x01,                          // hardware type: 1(ethernet)/2(IEEE802.2)
    0x08, 0x00,                          // protocol type
    6,                                   // hardware address length: MAC length
    4,                                   // protocol address length: IP length
    0x00, 0x01,                          // option code: 1(arp request)/2(arp response)/3(rarp request)/4
    0x00, 0x0c, 0x29, 0x8d, 0xd9, 0x0e,  // sender mac
    192, 168, 3, 91,                     // sender ip
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // receiver mac
    192, 168, 1, 1                     // receiver ip
    //47, 92, 164, 179                     // receiver ip
  };

  // 可以存储网卡信息的结构体
  struct ifreq ethreq;
  // 将需要使用的网卡名字拷入该结构体中
  strncpy(ethreq.ifr_name, "ens33", IFNAMSIZ);
  // 通过网卡名获取该网卡的信息
  if (ioctl(sock_raw_fd, SIOCGIFINDEX, &ethreq) == -1)
  {
    ERR_LOG("ioctl");
  }

  // 原始套接字发送数据需要使用的结构体
  struct sockaddr_ll sll;
  socklen_t slllen = sizeof(sll);
  bzero(&sll, slllen);
  sll.sll_ifindex = ethreq.ifr_ifindex;

  if (sendto(sock_raw_fd, arp_packet, M, 0, (struct sockaddr *)&sll, slllen) < 0)
  {
    ERR_LOG("sendto");
  }

  while(1)
  {
    unsigned char recv_msg[1024] = "";
    if (recvfrom(sock_raw_fd, recv_msg, 1024 ,0, NULL, NULL) < 0)
    {
      ERR_LOG("recvfrom");
    }

    if (ntohs(*(unsigned short *)(recv_msg + 12)) == 0x0806)
    {
      printf("link layer protocol: ARP\n");

      if (ntohs(*(unsigned short *)(recv_msg + 20)) == 2)
      {
        printf("This is a response packet.\n");
        unsigned char dst_mac[18] = "";

        sprintf(dst_mac, "%x:%x:%x:%x:%x:%x", \
                           recv_msg[22], recv_msg[23], recv_msg[24], \
                           recv_msg[25], recv_msg[26], recv_msg[27]);

        printf("dst_mac: %s\n", dst_mac);
        break;
      }
    }
  }

  return 0;
}

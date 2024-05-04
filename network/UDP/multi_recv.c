/*
 * author: inkXu
 * date:   2022.9.21
 *
 * 加入广播组，接收广播组消息
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "./recv multi_ip port\n");
    exit(1);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  char text[128] = "";

  struct ip_mreq mreq;
  memset(&mreq, 0, sizeof(mreq));

  mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
  mreq.imr_interface.s_addr = inet_addr("192.168.181.2");//INADDR_ANY;

  if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
  {
    perror("setsockopt");
    exit(1);
  }

  struct sockaddr_in multiaddr;
  memset(&multiaddr, 0, sizeof(multiaddr));

  socklen_t addrlen = sizeof(multiaddr);

  multiaddr.sin_family = AF_INET;
  multiaddr.sin_addr.s_addr = inet_addr(argv[1]);
  multiaddr.sin_port = htons(atoi(argv[2]));

  if (bind(sockfd, (struct sockaddr *)&multiaddr, sizeof(multiaddr)) < 0)
  {
    perror("bind");
    exit(1);
  }

  struct sockaddr_in sendaddr;
  memset(&sendaddr, 0, sizeof(sendaddr));

  while(1)
  {
    bzero(text, sizeof(text));

    if (recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&sendaddr, &addrlen) < 0)
    {
      perror("recvfrom");
      exit(1);
    }

    printf("[%s - %d]# %s\n", inet_ntoa(sendaddr.sin_addr), ntohl(sendaddr.sin_port), text);
  }

  close(sockfd);

  return 0;
}

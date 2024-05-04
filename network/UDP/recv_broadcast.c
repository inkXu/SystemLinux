/*
 * author: inkXu
 * date:   2022.9.20
 *
 * 接收广播
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define M 128

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    printf("./a.out port broadcast_ip\n");
    exit(1);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  socklen_t addrlen = sizeof(struct sockaddr_in);

#if 1
  struct sockaddr_in broadcastaddr;

  broadcastaddr.sin_family = AF_INET;
  broadcastaddr.sin_addr.s_addr = inet_addr(argv[2]);
  broadcastaddr.sin_port = htons(atoi(argv[1]));

  if (bind(sockfd, (struct sockaddr *)&broadcastaddr, addrlen) < 0)
  {
    perror("bind");
    exit(1);
  }
#endif

  char buf[M] = "";
  struct sockaddr_in sendaddr;

  while(1)
  {
    memset(buf, 0, sizeof(buf));

    if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&sendaddr, &addrlen) < 0)
    {
      perror("socket");
      exit(1);
    }

    printf("[%s - %d]# %s\n", inet_ntoa(sendaddr.sin_addr), ntohs(sendaddr.sin_port), buf);
  }

  close(sockfd);

  return 0;
}

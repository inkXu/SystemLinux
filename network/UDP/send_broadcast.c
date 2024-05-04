/*
 * author: inkXu
 * date:   2022.9.20
 *
 * 发送广播
 */
#include <stdio.h>         //printf
#include <stdlib.h>        //exit
#include <sys/socket.h>    //socket
#include <sys/types.h>
#include <netinet/in.h>    //sockaddr_in
#include <arpa/inet.h>     //htons  inet_addr
#include <unistd.h>        //close
#include <string.h>

#define M 128

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "./send ipaddr port\n");
    exit(1);
  }

  int sockfd;
  struct sockaddr_in broadcastaddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

#if 1
  int on = 1;

  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
  {
    perror("setsockopt");
    exit(1);
  }
#endif

  broadcastaddr.sin_family = AF_INET;
  broadcastaddr.sin_addr.s_addr = inet_addr(argv[1]);
  broadcastaddr.sin_port = htons(atoi(argv[2]));

  char buf[M] = "";

  while(1)
  {
    memset(buf, 0, sizeof(buf));

    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf) - 1] = '\0';

    if (sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&broadcastaddr, sizeof(broadcastaddr)) < 0)
    {
      perror("sendto");
      exit(1);
    }
  }

  close(sockfd);

  return 0;
}

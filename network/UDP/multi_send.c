/*
 * author: inkXu
 * date:   2022.9.21
 *
 * 向组播发送消息
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "./a.out multi_ip_addr port\n");
    exit(1);
  }

  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in multicastaddr;

  multicastaddr.sin_family = AF_INET;
  multicastaddr.sin_addr.s_addr = inet_addr(argv[1]);
  multicastaddr.sin_port = htons(atoi(argv[2]));

  char text[128] = "";

  while(1)
  {
    bzero(text, sizeof(text));

    fgets(text, sizeof(text), stdin);

    if (sendto(sockfd, text, sizeof(text), 0, (struct sockaddr *)&multicastaddr, sizeof(text)) < 0)
    {
      perror("sendto");
      exit(1);
    }
  }

  close(sockfd);

  return 0;
}

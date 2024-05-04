#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

void do_download(int sockfd, struct sockaddr_in serveraddr)
{
  char filename[128] = "";
  printf("please enter file name of you want download\n");
  scanf("%s", filename);

  unsigned char text[1024] = "";
  int text_len;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  int fd;
  int flags = 0;
  int num = 0;
  ssize_t bytes;

  text_len = sprintf(text, "%c%c%s%c%s%c", 0, 1, filename, 0, "octet", 0);
  if (sendto(sockfd, text, text_len, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    perror("sendto");
    exit(1);
  }

  while(1)
  {
    if ((bytes = recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&serveraddr, &addrlen)) < 0)
    {
      perror("recvfrom");
      exit(1);
    }
    if (text[1] == 5)
    {
      printf("error: %s\n", text + 4);
      return ;
    }
    else if(text[1] == 5)
    {
      if (flags == 0)
      {
        if ((fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0664)) < 0)
        {
          perror("open");
          exit(1);
        }
        flags = 1;
      }

      if ((num + 1 == ntohs(*(unsigned short *)(text + 2))) && (bytes == 516))
      {
        num = ntohs(*(unsigned short *)(text + 2));
        if (write(fd, text + 4, bytes - 4) < 0)
        {
          perror("write");
          exit(1);
        }

        text[1] = 4;
        if(sendto(sockfd, text, 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
        {
          perror("sendto");
          exit(1);
        }
      }
      else if ((num + 1 == ntohs(*(unsigned short *)(text + 2))) && (bytes < 516))
      {
        if (write(fd, text + 4, bytes - 4) < 0)
        {
          perror("write");
          exit(1);
        }

        text[1] = 4;
        if(sendto(sockfd, text, 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
        {
          perror("sendto");
          exit(1);
        }
        printf("file is download\n");
        return ;
      }
    }
  }
}

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
    exit(1);
  }

  int sockfd;
  struct sockaddr_in serveraddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket");
    exit(1);
  }

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(69);

  do_download(sockfd, serveraddr);

  return 0;
}

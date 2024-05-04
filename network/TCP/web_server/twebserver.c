#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define N 1024
#define ERR_LOG(errmsg) do{\
			  perror(errmsg);\
			  printf("%s - %s - %d\n", __FILE__, __func__, __LINE__);\
			  exit(1);\
			}while(0);

void *pfunc(void *sockfd);

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sockfd;
  struct sockaddr_in serveraddr, clientaddr;
  socklen_t addrlen = sizeof(struct sockaddr_in);

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    ERR_LOG("socket");
  }

  //将套接字设置为允许重复使用本机地址或设置为端口复用
  int on = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    ERR_LOG("setsockopt");
  }

  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(atoi(argv[2]));

  if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    ERR_LOG("bind");
  }

  if (listen(sockfd, 20) < 0)
  {
    ERR_LOG("listen");
  }

  int acceptfd;
  while(1)
  {
    if ((acceptfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
    {
      ERR_LOG("accept");
    }

    printf("%s:%d is Connected!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    pthread_t thread;
    if (pthread_create(&thread, NULL, pfunc, &acceptfd) != 0)
    {
      ERR_LOG("pthread_create");
    }
    pthread_detach(thread);
  }

  return 0;
}

void *pfunc(void *sockfd)
{
  int acceptfd = *(int *)sockfd;
  char buf[N] = "";
  char head[] = "HTTP/1.1 200 OK\r\n"         \
                   "Content-Type: text/html\r\n" \
                   "\r\n";
  char err[] = "HTTP/1.1 404 Not found\r\n"  \
                "Content-Type: text/html\r\n" \
                "\r\n"                        \
                "<html><body>File not found</body></html>";

  if (recv(acceptfd, buf, N, 0) < 0)
  {
    ERR_LOG("recv");
  }

  char filename[128] = "";
  sscanf(buf, "GET /%s", filename);

  if (strncmp(filename, "HTTP/1.1", strlen("http/1.1")) == 0)
  {
    strcpy(filename, "login.html");
  }
  printf("filename = %s\n", filename);

  char path[128] = "./about/";
  strcat(path, filename);

  int fd;

  if ((fd = open(path, O_RDONLY)) < 0)
  {
    if (errno == ENOENT)
    {
      if (send(acceptfd, err, sizeof(err), 0) < 0)
      {
        ERR_LOG("send");
      }
      close(acceptfd);
      pthread_exit(NULL);
    }
    else
    {
      ERR_LOG("open");
    }
  }

  if (send(acceptfd, head, strlen(head), 0) < 0)
  {
    ERR_LOG("send");
  }

  ssize_t bytes;
  char text[N];
  while((bytes = read(fd, text, N)) > 0)
  {
    if (send(acceptfd, text, bytes, 0) < 0)
    {
      ERR_LOG("send");
    }
    bzero(text, N);
  }
  pthread_exit(NULL);
}

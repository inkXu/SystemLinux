#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define M 1024
#define MSG_LOG(errmsg) do {\
                          perror(errmsg);\
                          exit(EXIT_FAILURE);\
                        } while(0);

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "%s ip_addr port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    MSG_LOG("socket");
  }

  int on = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
  {
    MSG_LOG("setsockopt");
  }

  struct sockaddr_in serveraddr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  memset(&serveraddr, 0, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
  serveraddr.sin_port = htons(atoi(argv[2]));

  if (bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
  {
    MSG_LOG("bind");
  }

  if (listen(sockfd, 20) < 0)
  {
    MSG_LOG("listen");
  }

  while(1)
  {
    int cfd;
    struct sockaddr_in clientaddr;
    if ((cfd = accept(sockfd, (struct sockaddr *)&clientaddr, &addrlen)) < 0)
    {
      MSG_LOG("accept");
    }

    printf("%s:%d is Connected!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    pthread_t thread;
    pthread_detach(thread);
    void *pfunc(void *sockfd);

    if (pthread_create(&thread, NULL, pfunc, (void *)&cfd) != 0)
    {
      MSG_LOG("pthread_create");
    }
  }

  close(sockfd);

  return 0;
}

void *pfunc(void *sockfd)
{
  int cfd = *(int *)sockfd;
  char text[M];
  bzero(text, M);
  if (recv(cfd, text, M, 0) < 0)
  {
    //MSG_LOG("recv");
    perror("recv");
    pthread_exit(NULL);
  }

  char buf[M] = "";
  if (strncmp(text, "GET", 3) == 0)
  {
    sscanf(text, "GET /%s", buf);
  }
  else if (strncmp(text, "POST", 4) == 0)
  {
    sscanf(text, "POST /%s", buf);
  }
  else
  {
    char err[] = "Connect refused!";
    if (send(cfd, err, sizeof(err), 0) < 0)
    {
      MSG_LOG("send");
    }
    close(cfd);
    pthread_exit(NULL);
  }

  int fd;
  char path[M] = "./exercicios/";
  char success[] = "HTTP/1.1 200 OK\r\n"         \
                   "Content-Type: text/html\r\n" \
                   "\r\n";
  char fail[] = "HTTP/1.1 404 Not found\r\n"  \
                "Content-Type: text/html\r\n" \
                "\r\n"                        \
                "<html><body>File not found</body></html>";

  if (send(cfd, success, sizeof(success), 0) < 0)
  {
    MSG_LOG("send");
  }

  strcat(path, buf);
  if ((fd = open(path, O_RDONLY)) < 0)
  {
    if (errno == ENOENT)
    {
      if (send(cfd, fail, sizeof(fail), 0) < 0)
      {
        MSG_LOG("send");
      }
      close(cfd);
      pthread_exit(NULL);
    }
    else
    {
      MSG_LOG("open");
    }
  }

  char fileMsg[M];
  ssize_t bytes;
  while((bytes = read(fd, fileMsg, M)) > 0)
  {
    if (send(cfd, fileMsg, M, 0) < 0)
    {
      MSG_LOG("send");
    }
    bzero(fileMsg, M);
  }
  pthread_exit(NULL);
}


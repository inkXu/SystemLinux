/*
 * author: inkXu
 * date:   2022.8.26
 *
 * 使用有名管道实现进程间通信2，类微信、QQ软件
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if (mkfifo("./sfifo", 0664) == -1 && mkfifo("./rfifo", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }

  int cpid = fork();
  if (cpid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (cpid > 0) {
    int rfd;
    if ((rfd = open("./sfifo", O_RDONLY)) == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    while(1) {
      char buf[1024] = "";
      int flag = read(rfd, buf, sizeof(buf));
      if (flag == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
      if (flag == 0) {
        exit(EXIT_FAILURE);
      }
      printf("receive message: %s", buf);
    }
    close(rfd);
  }
  else {
    int sfd;
    if ((sfd = open("./rfifo", O_WRONLY)) == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    while(1) {
      char buf[1024] = "";
      fgets(buf, sizeof(buf), stdin);
      if (write(sfd, buf, strlen(buf)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
    }
    close(sfd);
  }

  return 0;
}

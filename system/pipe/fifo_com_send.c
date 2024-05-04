/*
 * author: inkXu
 * date:   2022.8.26
 *
 * 使用有名管道实现进程间通信，类微信、QQ软件
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
  //创建两个管道文件sfifo和rfifo，若有同名的文件不创建
  //sfifo表示fifo_com_send.c文件的进行写入和fifo_com_recv.c文件的进行读取的管道文件
  //rfifo表示fifo_com_recv.c文件的进行写入和fifo_com_send.c文件的进行读取的管道文件
  if (mkfifo("./sfifo", 0664) == -1 && mkfifo("./rfifo", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }

  //创建父子进程，实现一个进程中可以同时进行写入和读取两个管道文件的功能
  int cpid = fork();
  if (cpid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (cpid > 0) {
    //父进程实现写入功能
    int sfd;
    //在这个地方打开sfifo管道文件，因为是有名管道，所以可以在不同进程中开启管道文件实现通信
    //打开一个管道文件，若是只写、只读方式打开管道文件，那么open会阻塞在此
    if ((sfd = open("./sfifo", O_WRONLY)) == -1) {
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
  else {
    int rfd;
    if ((rfd = open("./rfifo", O_RDONLY)) == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    while(1) {
      char buf[1024] = "";
      //这里的flag主要是为了防止另一个进程结束运行，而该进程还在读取管道的内容，只有读端，且管道中没有数据，
      //read会返回0值，所以需要对read返回值进行判断，当另一个进程结束后该进程不会进入死循环
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

  return 0;
}

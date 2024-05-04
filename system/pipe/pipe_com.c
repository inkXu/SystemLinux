/*
 * author: inkXu
 * date:   2022.8.24
 *
 * 使用无名管道实现父子进程间通信
 *
 * 由于无名管道创建后返回给当前进程两个文件描述符，这两个文件描述符为局部变量
 * 注意：
 * 利用无名管道进行进程间通信都是父进程创建无名管道，然后创建子进程，子进程会继承父进程的无名
   管道的文件描述符，然后父子进程通过读写无名管道实现进程间通信
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
  //管道需要在创建进程前创建，这样父子进程才能共有这个管道
  int pipefd[2];
#if 1
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  printf("pipefd[0] = %d\n", pipefd[0]);   //3，默认有标准输入0、输出1、错误2
  printf("pipefd[1] = %d\n", pipefd[1]);   //4
#endif

  int cpid = fork();

  if (cpid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (cpid > 0) {
#if 0
  //不能这样写，创建的管道在代码中算是局部存在的，它获取的是当前进程中的文件描述符个数，使用全局变量存放
  //pipe返回的文件描述符数字是由pipe所在的进程决定的
    if (pipe(pipefd) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
#endif
    char buf[1024] = "";
    while(1) {
      //scanf("%s", buf);
      //在没有写入值并确认时，父进程被阻塞
      fgets(buf, sizeof(buf), stdin);
      if (write(pipefd[1], buf, sizeof(buf)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
    }
  }
  else {
    char buf[1024] = "";
    while(1) {
      //因为无名管道内没有值，所以子进程会被阻塞
      if (read(pipefd[0], buf, sizeof(buf)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
      //fputs(buf, stdout);
      printf("ths value written by ths parent process is: %s", buf);
    }
  }

  return 0;
}

/*
 * author: inkXu
 * date:   2022.8.22
 *
 * pause函数
 *
 * 将进程挂起直至捕捉到信号为止，这个函数常用于判断信号是否已到。
 * 返回值：直到捕获到信号，pause返回值才返回-1，且errno被设置为EINTR
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
  pid_t pid = fork();

  if(pid < 0)
  {
    perror("fail to fork");
    exit(1);
  }
  else if(pid > 0)   //父进程代码区
  {
    printf("this is a parent process\n");
    //使用pause阻塞等待捕捉信号
    pause();
  }
  else
  {
    printf("this is a child process\n");
    sleep(2);
  }

  return 0;
}

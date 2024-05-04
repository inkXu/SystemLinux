/*
 * author: inkXu
 * date:   2022.8.18
 *
 * 进程的挂起
 *
 * sleep函数可以将当前进程转为等待态，等sleep执行完后，该进程会转为就绪态，而后转为执行态
 * 执行完毕返回0，中断时返回剩余的时间，参数以秒为单位
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  pid_t pid = fork();

  if( pid == -1 )
  {
    perror("fail to fork");
    return -1;
  }
  if( pid == 0 )
  {
    int i = 0;
    for(; i < 5; i++)
    {
      printf("this is a child process\n");
      sleep(1);
    }
  }
  else
  {
    int status = 0;
    printf("this is a parent process\n");
    wait(&status);
  }
  return 0;
}

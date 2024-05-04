/*
 * author: inkXu
 * date:   2022.8.22
 *
 * abort函数
 *
 * void abort(void)
 * 向程序发送一个SIGABRT信号，默认情况下进程会退出
 * 注意：即使SIGABRT信号被加入阻塞集，一旦进程调用了abort函数，进程还是会被终止，且在终止前会刷新缓冲区，
 * 关闭文件描述符
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
  pid_t pid = fork();

  if( pid < 0 )
  {
    perror("fail to fork");
    exit(1);
  }
  else if( pid == 0 )
  {
    int i = 0;
    for( ; i < 5; i++)
    {
      printf("welcome to china\n");
      //printf("ii");
      sleep(1);
      if(i == 3)
      {
        abort();
      }
    }
    exit(0);
  }
  else
  {
    sleep(1);
    printf("this is a parent process\n");
    wait(NULL);
  }

  return 0;
}

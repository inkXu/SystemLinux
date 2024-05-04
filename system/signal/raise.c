/*
 * author: inkXu
 * date:   2022.8.22
 *
 * raise函数
 *
 * 给调用该函数的进程发送一个信号
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[])
{
  int i = 0;

  while(1)
  {
    printf("thank you for your love\n");
    if( i == 5 )
    {
      raise(SIGINT);
      //等价于
      //kill(getpid(), SIGINT);
    }
    sleep(1);
    i++;
  }

  return 0;
}

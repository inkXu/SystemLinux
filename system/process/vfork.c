/*
 * author: inkXu
 * date:   2022.8.20
 *
 * vfork函数
 *
 * vfork和fork区别
 * vfork保证子进程先运行，在他调用exec或exit后，父进程才可能别调度运行
 * vfork和fork一样都创建一个子进程，但它并不将父进程的地址完全复制到子进程中，因为子进程会立即
   调用exec(或exit)，于是也就不访问该地址空间。(子父进程共用内存)
 * 相反，在子进程中调用exec或exit之前，它在父进程的地址空间中运行，在exec后子进程会有自己的进程空间。
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int a = 10;

int main(int argc, char *argv[])
{
  int b = 30;

  pid_t pid = vfork();

  if( pid < 0 )
  {
    perror("fail to vfork");
    exit(1);
  }
  else if( pid == 0 )
  {
    int i = 0;
    for(; i < 5; i++)
    {
      printf("this is a child process\n");
      sleep(1);
    }
    //
    //在执行exit或exec前父子进程共用内存，子进程中修改变量值，父进程调用该变量时也为改变后的值，fork函数不是这样
    a = a + 10;
    printf("this is a global variable: a = %d, b = %d\n", a, ++b);
    //使用vfork创建的子进程一定要写exit，子进程只有执行了exit或exec，父进程才会执行，如果父进程执行了可能会出错
    exit(0);
  }
  else
  {
    printf("this is a parent process\n");
    printf("this is a global variable: a = %d, b = %d\n", a, b);
  }

  return 0;
}

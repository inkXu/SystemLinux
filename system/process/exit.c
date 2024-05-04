/*
 * author: inkXu
 * date:   2022.8.20
 *
 * 进程的终止
 *
 * 有exit()和_exit()两个函数，用于结束或退出当前进程
 * 区别在于_exit()是系统调用，exit()是库函数，库函数有缓冲区，系统调用没有
 * 缓冲区的概念前面学了，使用\n，缓冲区满了，使用ffush，程序正常退出
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  printf("thank you");
  int i = 0;
  for(; i < 5; i++)
  {
    //exit会刷新缓冲区，_exit不会刷新缓冲区

    if(i == 3) exit(0);                   //执行结果：打印thank you

    //if(i == 3) _exit(0);                //执行结果：啥都不打印

    //在主函数中使用return也可以退出进程
  }
  printf("this is a for loop\n");

  return 0;
}

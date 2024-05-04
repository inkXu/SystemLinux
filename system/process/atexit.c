/*
 * author: inkXu
 * date:   2022.8.20
 *
 * 进程退出清理
 *
 * int atexit(void (*function)(void))
 * 注册进程正常结束前调用的函数，进程退出执行注册函数
 * 一个进程中可以多次调用atexit函数注册清理函数，正常结束前调用函数的顺序和注册时的顺序相反
   就是你一个进程调用了两次atexit，先执行的是下面的那个，在执行上一个atexit
 * 成功返回0，失败非0
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fun(void)
{
  printf("+----------------------------------------+\n");
  printf("|           welcome to china             |\n");
  printf("+----------------------------------------+\n");
}
void none(void)
{
  printf("atexit is called for second\n");
}
void show(void)
{
  printf("parent process is exit\n");
}

int main()
{
  printf("there is a block for main function\n");

  pid_t pid = fork();

  if( pid < 0 )
  {
    perror("fail to fork");
    exit(1);
  }
  else if( pid == 0 )
  {
    atexit(fun);
    atexit(none);
    int i = 0;
    for(i = 0; i < 5; i++)
    {
      printf("this is a child process\n");
      sleep(1);
    }
    exit(0);
  }
  else
  {
    atexit(show);
    wait(NULL);
    printf("this is a parent process\n");
  }

  return 0;
}

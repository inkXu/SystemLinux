/*
 * author: inkXu
 * date:   2022.8.18
 *
 * 进程等待
 *
 * wait，man手册有英文版
 * 功能：等待子进程终止，如果子进程终止了，此函数会回收子进程的资源。
 *       调用wait函数的进程会挂起，直到它的一个子进程退出或收到一个不能被忽视的信号才被
 *       若调用进程没有子进程或它的子进程已经结束，该函数立马返回。
 * 参数：status：函数返回时，参数status中包含子进程退出时的状态信息。
 *       子进程的退出信息在一个int中包含了多个字段，用宏定义可以取出其中的每个字段
 *       子进程可以通过exit或者_exit函数发送退出状态
 * 返回值：成功时为子进程pid，失败-1
 *
 * 取出子进程的退出信息
 *       WIFEXITED(status):
 *       如果子进程是正常终止的，取出的字段值非零。
 *       WEXITSTATUS(status):
 *       返回子进程的退出状态，退出保存在status变量的8-16位，在使用此宏之前应该先用
 *       WIFEXITED判断子进程是否正常退出，正常退出才能使用此宏
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fail to fork");
    return -1;
  }
  else if(pid == 0)              //子进程代码块
  {
    int i = 0;
    for(i = 0; i < 5; i++)
    {
      printf("this is a child process\n");
      sleep(1);
    }
    _exit(2);
  }
  else
  {
    //使用wait阻塞等待子进程执行完毕退出，若不想接收子进程状态，传一个NULL即可
    //wait(NULL);

    int status = 0;
    wait(&status);

    if(WIFEXITED(status) != 0)
    {
      printf("status value is %d\n", WEXITSTATUS(status));
    }
    printf("this is a parent process\n");
  }

  return 0;
}







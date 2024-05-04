/*
 * author: inkXu
 * date:   2022.8.22
 *
 * kill函数(man 2 kill)
 *
 * int kill(pid_t pid, int signum)
 * pid > 0：向进程号为pid的进程发送signum信号
 * pid = 0：向当前进程所在的进程组的所有进程发送signum信号
 * pid = -1：向当前系统执行的所有进程发送signum信号
 * pid < -1：向pid的绝对值的进程组的全部进程发送signum信号
 *
 * kill命令(man kill)
 * kill -9 进程号：-9表示数字编码为9的信号，为SIGKILL
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(int agrc, char *argv[])
{
  pid_t pid = fork();

  if( pid < 0 )
  {
    perror("fail to fork");
    exit(1);
  }
  else if( pid > 0 )
  {
    while(1)
    {
      char str[] = "this is a parent process\n";
      write(1, str, sizeof(str));
      sleep(1);
    }
  }
  else
  {
    sleep(3);

    //杀死当前系统下运行的所有进程
    //kill(-1, SIGKILL);

    //向父进程发送信号SIGINT信号，同Ctrl+c
    //kill(getppid(), SIGINT);

    //向父进程发送信号SIGQUIT信号，同Ctrl+\作用
    //kill(getppid(), SIGQUIT);

    //向父进程发送信号SIGTSTP信号，同Ctrl+z作用
    kill(getppid(), SIGTSTP);

    //向父进程发送信号SIGKILL信号，杀死父进程
    //kill(getppid(), SIGKILL);
    
    //向父进程发送信号SIGSTOP信号，停止父进程
    //kill(getppid(), SIGSTOP);

    char str[] = "this is a child process\n";
    write(1, str, sizeof(str));
  }
  return 0;
}

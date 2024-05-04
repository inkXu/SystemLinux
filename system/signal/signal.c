/*
 * author: inkXu
 * date:   2022.8.22
 *
 * signal函数
 *
 * typedef void (*sighandler_t)(int)                          //将sighandler_t定义为一个函数指针
 * sighandler_t signal(int signum, sighandler_t handler)
 * 第一个参数为int类型的信号，第二参数为该信号的处理方式，是一个函数指针，返回值也是该指针
 * 该函数可以更改信号的处理方式，其中hander即为更改后的方式，hander取值：
 * 传入SIG_IGN(忽略，ignore)，缺省该信号的处理方式
 * 传入SIG_DFL(默认，default)，使用默认的处理方式
 * 传入一个自定义的回调函数
 * 返回值：成功时为当前进程默认的处理方式，失败SIG_ERR
 *
 * 注意：SIGKILL(9)和SIGSTOP(19)的处理方式不能被更改，所以signal函数对这两个信号更改会报错
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void hande(int sig)
{
  printf("+---------------------------------+\n");
  printf("|  this is a customize function!  |\n");
  printf("+---------------------------------+\n");
}

void *sighandler_t = NULL;
void hander(int sig)
{
  //这里第二个参数直接写SIG_DFL也行
  //if( signal(SIGINT, SIG_DFL) == SIG_ERR )
  if( signal(SIGINT, sighandler_t) == SIG_ERR )
  {
    perror("fail to signal");
    exit(1);
  }
  printf("+-----------------------------------+\n");
  printf("|            game  start            |\n");
  printf("+-----------------------------------+\n");
}
int main(int argc, char *argv[])
{
#if 1
  //将SIGINT改为hander处理方式，sighandler_t会指向原来的SIGINT的默认处理方式
  if( (sighandler_t = signal(SIGINT, hander)) == SIG_ERR )
  {
    perror("fail to signal");
    exit(1);
  }
  while(1)
  {
    printf("I love you\n");
    sleep(1);
  }
#endif
#if 0
  //信号采用自定义的方式
  alarm(4);
  if( signal(SIGALRM, hande) == SIG_ERR )
  {
    perror("fail to signal");
    exit(1);
  }
  while(1)
  {
    printf("thank you for your love\n");
    sleep(1);
  }
#endif
#if 0
  //信号采用默认的处理方式
  if( signal(SIGALRM, SIG_DFL) == SIG_ERR )
  {
    perror("fail to signal");
    exit(1);
  }
  alarm(4);
  while(1)
  {
    printf("welcome to china\n");
    sleep(1);
  }
#endif
#if 0
  //信号采用缺省的方式，忽略该信号的作用
  pid_t pid = fork();

  if( pid < 0 )
  {
    perror("fail to fork");
    exit(1);
  }
  else if( pid > 0 )
  {
    //缺省SIGINT信号的作用，该信号被缺省后，使用Ctrl+c不能结束该进程
    if( signal(SIGINT, SIG_IGN) == SIG_ERR )
    {
      perror("fail to signal");
      exit(1);
    }
    if( signal(SIGQUIT, SIG_IGN) == SIG_ERR )
    {
      perror("fail to signal");
      exit(1);
    }
    if( signal(SIGTSTP, SIG_IGN) == SIG_ERR )
    {
      perror("fail to signal");
      exit(1);
    }
    //不能修改SIGKILL和SIGSTOP的处理方式
    //if( signal(SIGKILL, SIG_IGN) == SIG_ERR )
    //{
    //  perror("fail to signal");
    //  exit(1);
    //}
    while(1)
    {
      printf("this is a parent process\n");
      sleep(1);
    }
  }
  else
  {
    sleep(3);
    kill(getppid(), SIGINT);
  }
#endif

  return 0;
}

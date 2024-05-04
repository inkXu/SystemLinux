/*
 * author: inkXu
 * date:   2022.8.24
 *
 * 信号阻塞集
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[])
{
  sigset_t set;

  if( sigaddset(&set, SIGINT) == -1 )
  {
    perror("fail to sigaddset");
    exit(1);
  }
  //将Ctrl+c的信号SIGINT加入阻塞集中
  if(sigprocmask(SIG_BLOCK, &set, NULL) == -1)
  {
    perror("fail to sigprocmask");
    exit(1);
  }

  int i = 0;

  //在这个循环中使用Ctrl+c信号不会结束循环
  for(i = 0; i < 5; i++)
  {
    printf("test function sigprocmask\n");
    sleep(1);
  }

  //将SIGINT信号移出阻塞集，若上一个循环产生了SIGINT信号，那么上一个循环产生的SIGINT信号会在此
  //时生效，程序结束
  sigprocmask(SIG_UNBLOCK, &set, NULL);

  //该循环使用Ctrl+c时会立即生效SIGINT信号，程序停止运行
  for(i = 0; i < 5; i++)
  {
    printf("test function sigprocmask\n");
    sleep(1);
  }

  return 0;
}

/*
 * author: inkXu
 * date:   2022.8.22
 *
 * alarm函数
 *
 * unsigned int alarm(unsigned int seconds)
 * seconds秒后向调用该函数的进程发送SIGALRM信号，该信号默认操作是终止调用进程
 * 返回值：如果alarm函数之前没有alarm设置，则返回0，如果有返回上一个alarm剩余时间
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[])
{
#if 1
  //5秒后终止当前进程
  int sec = 0;

  sec = alarm(15);
  printf("sec = %d\n", sec);

  sleep(4);

  //第一个闹钟执行了4秒，执行第二个钟，第二个闹钟将第一个闹钟剩余的时间返回，
  //然后使用第二闹钟，第一个闹钟剩余的时间直接丢掉
  sec = alarm(4);
  printf("sec = %d\n", sec);

  while(1)
  {
    printf("thank you for your love\n");
    sleep(1);
  }
#endif

#if 0
  //与上面alarm(5)效果一样
  int i = 0;

  while(1)
  {
    printf("thank you for your love\n");
    sleep(1);
    i++;
    if( i == 5 )
    {
      //发送一个SIGALRM信号
      kill(getpid(), SIGALRM);
    }
  }
#endif

  return 0;
}

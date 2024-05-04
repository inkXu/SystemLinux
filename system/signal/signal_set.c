/*
 * author: inkXu
 * date:   2022.8.23
 *
 * 信号集
 * 
 * 一个用户进程常常需要对多个信号做出处理。为了方便对多个信号进行处理，linux中引用了信号集。
   信号集是用来表示多个信号的数据类型。
 * 信号集数据类型（sigset_t）在/usr/include/bits/sigset.h文件中有定义
 * 信号集相关的操作有以下几个：
 *   sigemptyset  sigaddset  sigdelset  sigismember  sigfillset
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void check(sigset_t *set, int sig)
{
#if 1
  //信号在信号集中返回1，不在返回0，错误返回-1
  if( sigismember(set, sig) == -1 )
  {
    perror("fail to sigismember");
    exit(1);
  }
  else if( sigismember(set, sig) == 1)
  {
    printf("the signal have this signal\n");
  }
  else
  {
    printf("the signal set does not have this signal\n");
  }
#endif
}

int main()
{
  //定义个信号集，信号集本质上是整型的结构体，使用一个数组存放信号级
  sigset_t set;

#if 1
  //初始化由set指向的信号集，清空所有信号，初始化为空信号集，成功返回0，失败-1
  if( sigemptyset(&set) == 0 )
  {
    printf("the signal set is inited\n");
  }
#endif

#if 0
  //初始化一个满的信号集，将信号集合设置为所有信号的集合。返回值同前
  if( sigfillset(&set) == 0 )
  {
    printf("the fill signal set is inited\n");
  }
#endif

  printf("before adding SIGINT\n");
  check(&set, SIGINT);

  //成功返回0，失败-1
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGTSTP);

  printf("after adding SIGINT\n");
  check(&set, SIGINT);

  sigdelset(&set, SIGINT);

  printf("after removing SIGINT\n");
  check(&set, SIGINT);

  return 0;
}

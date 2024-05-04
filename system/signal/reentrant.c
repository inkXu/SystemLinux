/*
 * author: inkXu
 * date:   2022.8.23
 *
 * 可重入函数
 *
 * 可重入函数就是可以被中断的函数，当前函数可以在任何时刻中断它，并执行另一块代码，执行完毕后，
   回到原来的代码还可以继续执行
 * 一个函数运行中，加入了其它代码块，这段代码块运行完毕后，原来的代码仍能继续运行的函数
 * 编写可重入函数
 *  不使用（返回）静态的数据、全局变量（除非用信号互斥）
 *  不调用动态内存分配、释放的函数
 *  不调用任何不可重入的函数（如标准I/O函数）
 * 注意：
 *  即使信号处理函数使用的都是可重入函数（常见的可重入函数），也要注意进入处理函数时，首先要保存errno的值，
 *  结束时再恢复原值。因为信号处理过程中，errno值随时可能改变。
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int sig)
{
  printf("\nSIGINT\n");
}

int main(int argc, char *argv[])
{
  signal(SIGINT, handler);

#if 1
  char str[32] = "";
  //read也是可重入函数，当在终端上输入数据时，使用Ctrl+c，发送一个SIGINT信号，read收到了该信号后，
  //原来的数据作废，read仍然可以继续读取数据，read函数继续运行
  if( read(0, str, 32) < 0 )
  {
    perror("fail to read");
    exit(1);
  }
  printf("str = %s\n", str);
#endif

#if 0
  //sleep就是一个可重入函数，但是当它执行信号处理函数后，不会回到原来的睡眠状态
  //sleep(10);

  //alarm的状态就比较明显，执行了信号处理函数后，它仍然可以继续执行
  alarm(5);

  while(1)
  {
    printf("welcome to china\n");
    sleep(1);
  }
#endif

  return 0;
}

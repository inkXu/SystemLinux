/*
 * author: inkXu
 * date:   2022.8.20
 *
 * 进程间的替换
 *
 * exec有execl execlp execle execv execvp execvpe六个函数族
 * 该函数组提供了六种在进程中启动另一个程序的方法
 * 该函数族可以根据指定的文件名或目录找到可执行文件
 * 调用exec函数的进程并不创建新的进程，故调用exec前后，进程的进程号并不会改变，其执行的程序
   完全由新的程序替代，而由exec开启的新程序则从其main函数开始执行。
 * 除了execvpe为系统调用，其余的都为库函数
 * exec后面字符的含义：
   l(list)：参数地址列表，以空指针结尾，参数地址列表
   v(vector)：存有各地址的指针数组的地址，指针数组存各参数的地址，然后将该指针数组作为函数的参数
   p(path)：按PATH环境变量指定的目录搜索可执行文件，以p结尾的exec函数取文件名作为参数，当指定filename为参数时
            若filename中包含/则将其视为路径名，并直接到指定的路径中执行程序。
   e(environment)：存有环境变量字符串地址的指针数组的地址。有e的函数会改变接下来程序执行的环境变量。
 * 执行exec函数族时，记得加上判断，因为它执行成功没有返回值，失败返回-1，程序会继续执行
 * 一个进程调用了exec后，除了进程ID，进程还保留了下列特征不变：
   父进程号、进程组号、控制终端、根目录、当前工作目录、进程信号屏蔽集、未处理信号...
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  printf("this program is execution\n");
#if 0
  //不带p的一定要写绝对路径
  if( execl("/bin/ls", "ls", "-l", NULL) == -1 )
  {
    perror("fail to execl");
    //return 1;
    //若是在子进程中，写exit好
    exit(1);
  }
#endif

#if 0
  //带p的函数，可以写成相对路径
  if( execlp("ls", "ls", "-l", NULL) == -1 )
  {
    perror("fail to execlp");
    exit(1);
  }
#endif

#if 0
  char *str[] = {"ls", "-l", NULL};
  if( execv("/bin/ls", str) == -1 )
  {
    perror("fail to execv");
    exit(1);
  }
#endif

#if 0
  //开启可c可执行文件可以使用不带p的函数，带p的也这样写，但是shell脚本不行，只能用execlp
  if( execl("./wait", "./wait", NULL) == -1 )
  {
    perror("fail to execl");
    exit(1);
  }
#endif

#if 0
  //开启可c可执行文件可以使用不带p的函数，带p的也这样写，但是shell脚本不行，只能用execlp
  if( execlp("./wait", "./wait", NULL) == -1 )
  {
    perror("fail to execlp");
    exit(1);
  }
#endif

#if 1
  //开启可c可执行文件可以使用不带p的函数，带p的也这样写，但是shell脚本不行，只能用execlp
  if( execlp("./exec.sh", "./exec.sh", NULL) == -1 )
  {
    perror("fail to execlp");
    exit(1);
  }
#endif

  //exec函数族取代调用进程中的代码段、数据段、BBS和栈堆段
  //所以当exec执行后，当前进程就结束了，程序不再执行
  printf("this program is terminate\n");

  return 0;
}

/*
 * author: inkXu
 * date:   2022.8.17
 *
 * 进程
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void test1();       //getpid(), getpgid(), getppid()
void test2();       //fork()
void test3();       //fork()，父子进程拥有独立的地址空间，对变量
void test4();       //fork()，子进程继承父进程的空间，对同一文件

int main(int argc, char *argv[])
{
  test4();
  return 0;
}

void test4()
{
  //子进程会继承父进程一些公有的区域，比如磁盘空间、内核空间
  //文件描述符的偏移量保存在内核空间中，所以父子进程之间读取同一文件时，读取值为另一个进程读取后偏移后的值
  int fd = 0;
  if((fd = open("./file.txt", O_RDONLY)) == -1)
  {
    perror("fail to open");
    return ;
  }
#if 1
  pid_t pid = fork();

  if(pid < 0)
  {
    perror("fail to fork");
    return ;
  }
  else if(pid > 0)
  {
#endif
    char buf[24] = "";
    printf("this is a parent process\n");
    if(read(fd, buf, 24) == -1)
    {
      perror("fail to read");
      return ;
    }
    printf("buf = [%s]\n", buf);
#if 1
  }
  else
  {
    char buf[24] = "";
    printf("this is a child process\n");
    if(read(fd, buf, 24) == -1)
    {
      perror("fail to read");
      return ;
    }
    printf("buf = [%s]\n", buf);
  }
  //因为父进程先运行完了，所以会出现在两个进程间打印了终端提示符，而子进程运行完没有终端提示符的情况
  //所以加一个while，防止其打印终端提示符
  while(1)
  {}
#endif
}

void test3()
{
  //子进程会继承fork之前的所有内容
  //但是fork之后，父子进程完全独立，不管对方怎么改变(栈区、堆区、数据区等)，都不会收到对方的数据
  int a = 100, b = 888, c = 999;
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fail to fork");
    return ;
  }
  else if(pid > 0)
  {
    //为了防止父进程先执行了
    sleep(1);
    printf("this is a parent process\n");
    printf("a = %d, b = %d, c = %d\n", a, b, c);
  }
  else
  {
    a++;
    b++;
    c++;
    printf("this is a son process\n");
    printf("a = %d, b = %d, c = %d\n", a, b, c);
  }
}

void test2()
{
  /*
   * 使用fork函数得到的子进程是父进程的一个复制品，它从父进程处继承了整个进程的地址空间。
   * 地址空间：包括进程上下文、进程堆栈、打开的文件描述符、信号控制设定、进程优先级、进程组号等。子进程所独有的
     只有它的进程号，计时器等。因此使用fork代价是很大的
   */
#if 0
/*
 * 未区分父子进程
 * 执行一次fork后就会在原有的进程基础上创建一个新的子进程
 * 而且使用fork后若没有区分父子进程的代码区，则后面所有的代码都会一起执行
 */
  fork();
  while(1)
  {
    printf("pid = %d, ppid = %d\n", getpid(), getppid());
    //给它来了个时间片的感觉，没有sleep只看到它执行了父进程
    sleep(1);
  }
#endif
#if 1
  //fork()用于创建一个子进程
  //返回值大于0是子进程id，等于0是父进程，小于0表示创建子进程失败，失败时会给errno赋值
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fail to fork");
    return ;
  }
  else if(pid == 0)   //fork返回值为0时表示父进程的pid，下面是子进程的代码区
  {
    while(1)
    {
      printf("pid = %d\n", getpid());
      printf("ppid = %d\n", getppid());
      printf("this is son process\n");
      //使用sleep让该进程进入等待态，而后该进程并没有执行完再执行另一个进程
      sleep(1);
      printf("++++++++++++++++++++++++++\n");
    }
  }
  else              //fork返回值大于0时表示子进程的pid，下面是父进程的代码区
  {
    while(1)
    {
      printf("pid = %d\n", getpid());
      printf("ppid = %d\n", getppid());
      printf("this is parent process\n");
      sleep(1);
      printf("========================\n");
    }
  }
#endif
}

void test1()
{
  //获取当前进程父进程id
  printf("getppid = %d\n", getppid());
  //获取当前进程id
  printf("getpid = %d\n", getpid());
  //获取当前进程组id
  printf("getpgid = %d\n", getpgid(getpid()));
  //用于停滞程序
  while(1){}
}

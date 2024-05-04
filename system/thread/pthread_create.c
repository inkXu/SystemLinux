/*
 * author: inkXu
 * date:   2022.8.28
 *
 * 线程的创建
 *
 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
 *                           void *(*start_routine) (void *), void *arg);
 * thread：子线程id
 * attr：线程的创建方式，一个结构体指针，写NULL就是默认的方式创建
 * start_routine：该函数指针为线程的函数，函数指针类型看上面的声明，线程函数的入口地址
 * arg：该指针是前一个参数的参数，函数指针的参数
 * 成功返回0，失败不为0
 *
 * 就像每个进程都有进程号一样，每个线程也有一个线程号
 * 进程号在每个系统中是唯一的，但线程号不同，线程号只在它所处的进程环境中有效
 * 进程号用pid_t数据类型表示，是一个非负整数，线程号则用pthread_t数据类型来表示
 * 有的系统在实现pthread_t的时候使用一个结构体来表示，所以在可移植的操作系统实现不能把它作为整数处理
 * 与fork不同的是pthread_create创建的线程不与父进程在同一点开始运行，而是从指定的函数开始运行，该函数
   运行完后，该线程也就退出了
 * 线程依赖进程存在的，如果创建线程的进程结束了，线程也就结束了
 * 线程函数的程序在pthread库中，由于线程库原本不是系统本身的，所以在链接时(gcc -o时)需要手动链接库文件
                                         gcc -o test test.c -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int num = 666;

//线程处理函数可以认为就是一个普通的全局函数，与普通函数最大的区别在于线程处理函数是并行执行，来回交替执行
void *thread_fun1(void *arg);
void *thread_fun2(void *arg);
void *thread_fun(void *arg);
void *thread_fun3(void *arg);

int main(int argc, char *argv[])
{
  //一个进程中的多个线程执行顺序是不确定的，没有先后顺序
  //下面三个线程不是按顺序执行的，和父子进程的调度机制一样，时间片轮转，上下切换

  int arg = 520;
  pthread_t id1, id2, id3;
#if 0
  if (pthread_create(&id1, NULL, thread_fun1, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }
#endif

  //if (pthread_create(&id2, NULL, thread_fun2, (void *)3) != 0) {
  if (pthread_create(&id2, NULL, thread_fun2, (void *)&arg) != 0) {
    perror("pthread_create");
    exit(1);
  }
  if (pthread_create(&id2, NULL, thread_fun, (void *)&arg) != 0) {
    perror("pthread_create");
    exit(1);
  }

#if 0
  char name[8] = "thank";
  if (pthread_create(&id3, NULL, thread_fun3, (void *)name) != 0) {
    perror("pthread_create");
    exit(1);
  }
#endif

  //若是没有sleep，则程序会直接结束，那么线程就会直接关闭，因为进程结束了，也可以使用while让进程进入死循环
  //不然子线程还没有运行主控线程就结束了，进程结束后，进程中的所有线程会强制退出
  sleep(3);

  return 0;
}

void *thread_fun3(void *arg) {
  printf("this is thread 3\n");
  sleep(1);
  printf("num = %d\n", num++);
  printf("arg = %s\n", arg);
}

void *thread_fun2(void *arg) {
  printf("this is thread 2\n");
  sleep(1);
  printf("num = %d\n", num++);

  //若是传入的是一个变量，不能直接用arg输出，得使用一个局部变量接收值再输出，若是传入一个常量就能用arg输出
  printf("arg = %d\n", *(int *)arg);
  //传入常量和变量都能用，但不知道会不会有问题，这个赋值操作好像只是一个局部变量，并没有修改到指针的值
  //arg = (void *)101;
  //printf("arg = %d\n", arg);

  //int *n = (int *)arg;

  //老师讲的，他能用，我不能，程序会奔溃，我是直接传入一个整数，将整数强转为void *类型，所以不能下面这样写
  //int n = *(int *)arg;
  //printf("arg = %d\n", n);
  *(int *)arg = 1111;
  printf("arg = %d\n", *(int *)arg);
}

void *thread_fun(void *arg) {
  sleep(2);
  printf("arg = %d\n", *(int *)arg);
}

void *thread_fun1(void *arg) {
  printf("this is a thread\n");
  sleep(1);
  printf("num = %d\n", num++);
  printf("-----------------------------\n");
}

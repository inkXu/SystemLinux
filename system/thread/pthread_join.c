/*
 * author: inkXu
 * date:   2022.8.29
 *
 * 线程的等待
 *
 * int pthread_join(pthread_t thread, void **retval);
 * 等待子线程的结束，并回收子线程资源
 * thread：被等待的线程号
 * retval(return value)：线程函数的返回值，没有返回值传入一个NULL即可，用来存储线程退出状态的指针地址
 * 成功返回0，失败返回非0
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_fun(void *arg);
void *thread_fun1(void *arg);
void *thread_fun2(void *arg);

int main(int argc, char *argv[])
{
  printf("this process is start\n");

#if 1
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun2, 0) != 0) {
    perror("pthread_create");
    exit(1);
  }
  int *num = NULL;
  if (pthread_join(thread, (void **)&num) != 0) {
    perror("pthread_create");
    exit(1);
  }
  printf("num = %d\n", *num);
#endif 

#if 0
  pthread_t thread;
  if (pthread_create(&thread, NULL, thread_fun1, 0) != 0) {
    perror("pthread_create");
    exit(1);
  }

  //第二个参数为一个二级指针
  char *str = NULL;
  if (pthread_join(thread, (void **)&str) != 0) {
    perror("pthread_join");
    exit(1);
  }
  //打印字符串指针的内容时，str不用加*，虽说*str表示内容
  printf("the return value is: %s\n", str);
#endif

#if 0
  //pthread_join函数阻塞等待线程的结束，线程语句执行完毕或return或使用了pthread_exit函数标志线程结束
  //线程无返回值
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun, 0) != 0) {
    perror("pthread_create");
    exit(1);
  }

  //通过调用pthread_join函数阻塞等待子线程退出
  if (pthread_join(thread, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }
#endif

  printf("this process is terminate\n");

  return 0;
}

void *thread_fun2(void *arg) {
  static int num = 100;
  return (void *)&num;
}

void *thread_fun1(void *arg) {
  //局部变量，默认是在栈区开辟空间，出了该函数范围会被释放，所以返回值要在静态全局区或堆区开辟空间
  static char str[16] = "thank you";
  printf("this is thread 1\n");
  //子线程如果要返回退出状态，可以通过返回值或者使用pthread_exit函数，记得加(void *)
  return (void *)str;
}

void *thread_fun(void *arg) {
  printf("this is a thread\n");
  sleep(2);
  printf("----------------------\n");
}

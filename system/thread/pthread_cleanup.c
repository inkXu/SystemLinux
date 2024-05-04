/*
 * author: inkXu
 * date:   2022.8.30
 *
 * 线程清理函数
 *
 * void pthread_cleanup_push(void (*routine)(void *), void *arg);
 * 将清理函数压栈，即注册清理函数
 * routine:线程清理函数的指针
 * arg:传给线程清理函数的参数
 *
 * void pthread_cleanup_pop(int execute);
 * 将清理函数弹栈，即删除清理函数
 * execute：线程清理函数执行标志位，非0弹出清理函数，执行清理函数，0，弹出清理函数，不执行清理
 *
 * 当线程执行以下操作时会调用清理函数
 * 1）调用pthread_exit退出进程时
 * 2）相应其它线程的取消请求时（pthread_cancel）
 * 3）用非0execute调用pthread_cleanup_pop
 * 无论哪种情况pthread_cleanup_pop都将删除上一次pthread_cleanup_push调用注册的清理处理函数
 *
 * 类似进程清理函数atexit，线程结束后才执行的代码
 * 线程可以建立多个清理处理函数
 * 处理程序在栈中，故它们的执行顺序与它们注册时的顺序相反
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void *thread_fun(void *arg);
void clean_up(void *arg);
void *thread_fun1(void *arg);
void *thread_fun2(void *arg);
void cleanup(void *arg);

int main(int argc, char *argv[])
{
#if 1
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun2, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }
#endif

#if 0
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun1, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  sleep(5);

  if (pthread_cancel(thread) != 0) {
    perror("pthread_create");
    exit(1);
  }
#endif

#if 0
  //通过pthread_exit退出进程时，线程清理函数会被调用
  pthread_t thread;
  if (pthread_create(&thread, NULL, thread_fun, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }
#endif

  //阻塞等待进程结束
  if (pthread_join(thread, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  return 0;
}

void cleanup(void *arg) {
  printf("cleanup arg = %s\n", (char *)arg);
  free((char *)arg);
}
void *thread_fun2(void *arg) {
  printf("this is a thread\n");

  char *ptr1 = (char *)malloc(32);
  bzero(ptr1, 32);
  strcpy(ptr1, "welcome");
  char *ptr2 = (char *)malloc(32);
  bzero(ptr2, 32);
  strcpy(ptr2, "thanks");

  //先压栈的是ptr1字符串，所以先弹栈的是ptr2字符串的内容，先进后出
  pthread_cleanup_push(clean_up, ptr1);
  pthread_cleanup_push(cleanup, ptr2);
  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);
}
void *thread_fun1(void *arg) {
  printf("this is a thread\n");

  char *ptr = (char *)malloc(32);
  bzero(ptr, 32);
  strcpy(ptr, "thank you");

  pthread_cleanup_push(clean_up, ptr);

  sleep(10);

  printf("The child thread is terminate\n");

  pthread_exit(NULL);

  pthread_cleanup_pop(1);
}
void clean_up(void *arg) {
  printf("clean up arg = %s\n", (char *)arg);
  //注意这里的arg要进行强制转换，因为用malloc申请地址的时候申请的是char类型的
  free((char *)arg);
}
void *thread_fun(void *arg) {
  printf("this is a thread\n");

  char *ptr = (char *)malloc(64);
  bzero(ptr, 64);      //memset(ptr, 0, sizeof(64))
  strcpy(ptr, "The memory of malloc is freed");

  //pthread_cleanup_push和pthread_cleanup_pop成对存在，都写在一个线程函数里面
  pthread_cleanup_push(clean_up, ptr);

  pthread_exit(NULL);
  //不论你通过什么方式执行线程清理函数，都必须写pthread_cleanup_pop函数，它们两个是成对存在的
  pthread_cleanup_pop(1);
}











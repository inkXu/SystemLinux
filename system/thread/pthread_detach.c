/*
 * author: inkXu
 * date:   2022.8.29
 *
 * 线程的分离
 *
 * int pthread_detach(pthread_t thread);
 * 使调用线程与当前线程分离，使其成为一个独立的线程，该线程终止时，系统将自动回收其资源
 * 成功返回0，失败返回非0
 *
 * 线程的结合态和分离态
 * linux线程执行和Windows不同，pthread有两种状态，可结合的（joinable）或者是分离的（detached），线程创建
   默认为可结合态。如果线程是joinable状态，当线程函数自己返回退出时或pthread_exit时都不会释放线程所占用
   堆栈和线程描述符（总计8k多），只有调用了pthread_join后这些资源才会被释放
 * 若是detached状态的线程，这些资源在线程函数退出时或pthread_exit时自动会被释放，使用pthread_detach函数将
   线程设置为分离态
 * 创建一个线程后应回收其资源，但使用pthread_join函数会使调用者发生阻塞，故linux提供了pthread_detach函数
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_fun(void *arg);

int main(int argc, char *argv[])
{
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun, 0) != 0) {
    perror("pthrad_create");
    exit(1);
  }

  //将线程号为thread的线程设为分离态，分离态下，不需要等待子线程执行结束再执行进程的代码
  if (pthread_detach(thread) != 0) {
    perror("pthread_detach");
    exit(1);
  }
/*
  //分离态下，线程的资源占用会自动被释放，不要写pthread_join来释放资源和接收返回值，它返回值不为0
  //如果是结合态，就需要使用pthread_join函数来回收子线程资源，但是该函数是一个阻塞函数，如果子线程
  //不退出，就会导致当前进程（主控线程）无法继续执行，大大限制了代码的运行效率
  int *num = NULL;
  if (pthread_join(thread, (void **)&num) != 0) {
    perror("pthread_join");
    exit(1);
  }
  printf("num = %d\n", num);
*/

  while(1) {
    printf("this is main thread\n");
    sleep(1);
  }

  return 0;
}

void *thread_fun(void *arg) {
  static int num = 2;
  printf("welcome to china\n");
  sleep(3);
  printf("thank you\n");
  return (void *)&num;
}






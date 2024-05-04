/*
 * author: inkXu
 * date:   2022.8.29
 *
 * 线程退出
 *
 * void pthread_exit(void *retval);
 * retval：返回值
 * 注意：一个进程中的多个线程是共享该进程的数据段，因此，通常线程退出后所以占用的资源并不会释放，
 *       如果要释放资源，结合态需要通过pthread_join函数，分离态则自动释放
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_fun(void *arg);

int main()
{
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun, 0) != 0) {
    perror("pthread_create");
    exit(1);
  }

  char *str = NULL;
  if (pthread_join(thread, (void **)&str) != 0) {
    perror("pthread_join");
    exit(1);
  }

  printf("the return value of thread is: %s\n", str);

  return 0;
}

void *thread_fun(void *arg) {
  static char str[32] = "welcome to china";
  printf("this is a thread\n");
  int i = 0;
  while(1) {
    if (i == 5) {
      pthread_exit(&str);
    }
    printf("i = %d\n", i);
    i++;
    sleep(1);
  }
}

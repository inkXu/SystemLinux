/*
 * author; inkXu
 * date:   2022.9.3
 *
 * 信号量操作
 *
 * 线程异步就是让一个线程先运行完在运行另一个线程，和互斥锁差不多的
 * 线程同步就是可以规定哪个线程先执行，哪个线程后执行
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

char ch = 'a';

sem_t sem_g, sem_p;
sem_t sem;

void *thread_func1(void *arg);
void *thread_func2(void *arg);
void *thread_func3(void *arg);
void *thread_func4(void *arg);
void *printer(char *arg);

int main(int argc, char *argv[])
{
#if 1
  //使用信号量实现异步操作
  pthread_t thread3, thread4;

  if (sem_init(&sem, 0, 1) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&thread3, NULL, thread_func3, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&thread4, NULL, thread_func4, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);

  putchar('\n');

  sem_destroy(&sem);
#endif

#if 0
  //使用信号量实现进程间同步操作
  pthread_t thread1, thread2;
  sem_init(&sem_g, 0, 0);
  sem_init(&sem_p, 0, 1);

  if (pthread_create(&thread1, NULL, thread_func1, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&thread2, NULL, thread_func2, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  sem_destroy(&sem_g);
  sem_destroy(&sem_p);
#endif

  return 0;
}

void *printer(char *arg) {
  sem_wait(&sem);
  while(*arg) {
    putchar(*arg);
    fflush(stdout);
    arg++;
    sleep(1);
  }
  sem_post(&sem);
}
void *thread_func3(void *arg) {
  char str[] = "hello";
  printer(str);
}
void *thread_func4(void *arg) {
  char str[] = "world";
  printer(str);
}
void *thread_func1(void *arg) {
  while(ch) {
    sem_wait(&sem_p);
    //自动类型转换putchar(int ch);
    putchar(ch);
    fflush(stdout);
    sem_post(&sem_g);
  }
}
void *thread_func2(void *arg) {
  while(ch) {
    sem_wait(&sem_g);
    ch++;
    sleep(1);
    sem_post(&sem_p);
  }
}


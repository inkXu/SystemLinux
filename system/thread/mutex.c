/*
 * author: inkXu
 * date:   2022.8.30
 *
 * 使用互斥锁完成主副银行卡取钱操作
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//两张银行卡共有10000元，都可以用来取钱
int money = 10000;
//声明一个互斥锁，如果不定义为全局变量话还需要将互斥锁传入线程内，太麻烦了，所以使用全局变量
pthread_mutex_t mymutex;

void *thread_func1(void *arg);
void *thread_func2(void *arg);
//没有互斥锁时
void bank_operator(char *name, int get);
//有互斥锁时
void bank_operator_mutex(char *name, int get);

int main(int argc, char *argv[])
{
  //在使用互斥锁前，需要初始化互斥锁
  //pthread_mutex_init(&mymutex, NULL);

  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, thread_func1, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&thread2, NULL, thread_func2, NULL) != 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  //阻塞等待线程thread1和thread2
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  //对互斥锁进行销毁，要记得写
  //pthread_mutex_destroy(&mutex);

  return 0;
}

void *thread_func1(void *arg) {
  //bank_operator("main_bank_card", 10000);
  bank_operator_mutex("main_bank_card", 10000);
}
void *thread_func2(void *arg) {
  //bank_operator("secondly_bank_card", 10000);
  bank_operator_mutex("secondly_bank_card", 10000);
}
void bank_operator_mutex(char *name, int get) {
  //对互斥锁进行上锁操作，发现好像和全局变量没有任何关系，锁住的是对该变量的操作
  pthread_mutex_lock(&mymutex);

  //若是定义了局部变量来存放全局变量的值就需要使用互斥锁才能保证实际效果
  int balance = money;

  printf("The %s is used\n", name);
  printf("The system is loading...\n");
  sleep(1);

  printf("Take out %d$, system is loading...\n", get);
  sleep(1);

  if (balance >= get) {
    balance = balance - get;
    money = balance;
    printf("Success, you took out %d$, the balance is: %d$\n", get, balance);
  }
  else {
    printf("Fail, you balance is not enough!\n");
  }

  //对互斥锁进行解锁操作
  pthread_mutex_unlock(&mymutex);
}
void bank_operator(char *name, int get) {
  //若是定义了局部变量来存放全局变量的值就需要使用互斥锁才能保证实际效果
  int balance = money;

  printf("The %s is used\n", name);
  printf("The system is loading...\n");
  sleep(1);

  printf("Take out %d$, system is loading...\n", get);
  sleep(1);

  if (balance >= get) {
    balance = balance - get;
    money = balance;
    printf("Success, you took out %d$, the balance is: %d$\n", get, balance);
  }
  else {
    printf("Fail, you balance is not enough!\n");
  }
}


















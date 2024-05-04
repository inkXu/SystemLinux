/*
 * author: inkXu
 * date:   2022.8.29
 *
 * 线程取消
 *
 * int pthread_cancel(pthread_t thread);
 * 将一个线程取消，实质是发信号给目标线程thread，使目标线程退出，此函数只是发送终止信号给目标线程，不会等待
   目标线程执行完才返回。但是，并不是发送成功就意味着目标线程一定就会终止，线程被取消时，线程的取消属性会决
   定线程能否被取消以及何时取消。
 *
 * int pthread_setcancelstate(int state, int *oldstate);
 * 设置取消状态，Linux系统下，线程默认是可以被取消，可以通过这个函数设置线程是否可以被取消
 * state:PTHREAD_CANCEL_ENABLE(可以取消)    PTHREAD_CANCEL_DISABLE(不可以取消)
 * oldstate:保存调用线程原来的可取消状态的内存地址
 *
 * int pthread_setcanceltype(int type, int *oldtype);
 * 设置取消类型，在线程能被取消的状态下，是立马取消结束还是执行到取消点的时候被取消结束
 * type:PTHREAD_CANCEL_DEFERRED(不立即取消)   PTHREAD_CANCEL_ASYNCHRONOUS(立即取消)
 * oldstate:保存调用线程原来的可取消状态的内存地址
 *
 * void pthread_testcancel(void);
 * 设置线程的取消点，线程被取消后，该线程并不是马上终止，默认情况下线程执行到取消点才能被终止，可以使用该函
   设置线程的取消点。当别的线程调用pthread_cancel取消当前线程时，当前线程执行到该函数才结束该线程
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread_fun(void *arg);
void *thread_fun1(void *arg);
void *thread_fun2(void *arg);
void *thread_fun3(void *arg);

int main()
{
#if 1
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun3, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  sleep(3);

  if (pthread_cancel(thread) != 0) {
    perror("pthread_cancel");
    exit(1);
  }
#endif

#if 0
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun2, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  sleep(3);

  if (pthread_cancel(thread) != 0) {
    perror("pthread_cancel");
    exit(1);
  }
#endif
#if 0
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun1, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_cancel(thread) != 0) {
    perror("pthread_cancel");
    exit(1);
  }
#endif

#if 0
  pthread_t thread;

  if (pthread_create(&thread, NULL, thread_fun, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  sleep(3);

  //取消线程thread
  if (pthread_cancel(thread) != 0) {
    perror("pthread_cancel");
    exit(1);
  }
#endif

  if (pthread_join(thread, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  return 0;
}

void *thread_fun3(void *arg) {
  while(1) {
    printf("this thread uses thread_testcancel\n");
    sleep(1);
    //感觉没有什么区别，设置和没设置好像一样
    pthread_testcancel();
  } 
}
void *thread_fun2(void *arg) {
  //默认类型PTHREAD_CANCEL_ASYNCHRONOUS为异步类型
  /*
  if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }
  */
  //延迟取消线程，不过这个延迟的时间特别短，看不出来
  if (pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }
  while(1) {
    printf("The type of this thread is deferred\n");
    sleep(1);
  }
}
void *thread_fun1(void *arg) {
  //将该线程设置为可取消线程，默认为该状态
  //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  //将该线程设置为不可取消，设置后，如果pthread_cancel函数向该线程发送取消信号，该线程不会结束运行
  if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  while(1) {
    printf("The status of this thread is not cancelable\n");
    sleep(1);
  }
  pthread_exit(NULL);
}
void *thread_fun(void *arg) {
  while(1) {
    printf("this is a child thread\n");
    sleep(1);
  }
  pthread_exit(NULL);
}





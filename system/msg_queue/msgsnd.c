/*
 * author: inkXu
 * date:   2022.8.27
 *
 * 发送消息
 *
 * int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
 * msqid：消息队列的标识msqid，使用ipcs -q可以查看到，该表中有一列是msqid
 * msgp：存放消息的结构体，该结构体必须声明一个long类型的消息类型，且该类型一定要在结构体的第一个参数
 * msgsz：存放消息的大小，前一个参数的结构体中，除了消息类型其余部分的大小sizeof(struct MSG)-sizeof(long)
 * msgflg：函数的控制属性，设置为0时，msgsnd调用阻塞直到条件满足为止
                           设置为IPC_NOWAIT时，若消息没有立即发送则调用该函数的进程会立即返回
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define N 64

typedef struct {
  //消息队列的结构体第一个参数一定是long类型的消息类型
  long mtype;
  char mtext[N];
} MSG;

//注意：msgsnd的第三个参数大小是结构体中除了消息类型后的大小
#define MSGTEXT_SIZE (sizeof(MSG) - sizeof(long))

int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }

  int msqid;

  if ((msqid = msgget(key, IPC_CREAT | 0664)) == -1) {
    perror("msgget");
    exit(1);
  }

  MSG msg1 = {1, "welcome to china"};
  MSG msg3 = {3, "I love you"};
  MSG msg2 = {2, "thank you for you love"};
  MSG msg4 = {4, "kiana"};

  if (msgsnd(msqid, &msg1, MSGTEXT_SIZE, 0) == -1) {
    perror("msgsnd");
    exit(1);
  }
  if (msgsnd(msqid, &msg2, MSGTEXT_SIZE, 0) == -1) {
    perror("msgsnd");
    exit(1);
  }
  if (msgsnd(msqid, &msg3, MSGTEXT_SIZE, 0) == -1) {
    perror("msgsnd");
    exit(1);
  }
  if (msgsnd(msqid, &msg4, MSGTEXT_SIZE, 0) == -1) {
    perror("msgsnd");
    exit(1);
  }

  //第四个参数置为0表示使用默认的阻塞方式，使用IPC_NOWAIT时，表示设置为非阻塞
  //if (msgsnd(msqid, &msg4, MSGTEXT_SIZE, IPC_NOWAIT) == -1) {

  return 0;
}

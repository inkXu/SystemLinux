/*
 * author: inkXu
 * date:   2022.8.27
 *
 * 读取消息
 *
 * ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
 * msqid：消息队列的标识msqid，使用ipcs -q可以查看到，该表中有一列是msqid
 * msgp：存放消息的结构体，该结构体必须声明一个long类型的消息类型，且该类型一定要在结构体的第一个参数
 * msgsz：存放消息的大小，前一个参数的结构体中，除了消息类型其余部分的大小sizeof(struct MSG)-sizeof(long)
 * msgtyp：写入消息队列时结构体的long类型的消息类型
           > 0：指定某个消息类型，若是消息队列中没有该消息类型，则发生阻塞
           = 0：按照写入消息队列的顺序逐个读取
           < 0：若是消息队列中有小于等于magtyp绝对值的消息，则取其中最小的消息，若是没有则发生阻塞
 * msgflg：函数的控制属性，设置为0时，msgsnd调用阻塞直到条件满足为止
                           设置为IPC_NOWAIT时，若消息没有立即发送则调用该函数的进程会立即返回
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define N 64

typedef struct {
  long mtype;
  char mtext[N];
} MSG;

#define MSGTEXT_SIZE (sizeof(MSG) - sizeof(long))


int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }

  int msqid;

  if ((msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
    perror("msgget");
    exit(1);
  }

  MSG msg;
  memset(&msg, 0, sizeof(msg));

#if 0
  //若是存在消息类型小于等于3的消息，则读取这些消息类型中最小的消息，若是没有了，则产生信号ENOMSG
  if (msgrcv(msqid, &msg, MSGTEXT_SIZE, -3, IPC_NOWAIT) == -1) {
    perror("msgrcv");
    exit(1);
  }
#endif

#if 0
  //若是存在消息类型小于等于3的消息，则读取这些消息类型中最小的消息，若是没有了，则发生阻塞
  if (msgrcv(msqid, &msg, MSGTEXT_SIZE, -3, 0) == -1) {
    perror("msgrcv");
    exit(1);
  }
#endif

#if 0
  //读取消息类型为3的消息，若是没有，则发生阻塞，直到有该消息恢复运行
  if (msgrcv(msqid, &msg, MSGTEXT_SIZE, 3, 0) == -1) {
    perror("msgrcv");
    exit(1);
  }
#endif

#if 1
  //最后一个参数设置为IPC_NOWAIT表示非阻塞，在这里，若是消息队列中没有消息则输出No message of desired type
  if (msgrcv(msqid, &msg, MSGTEXT_SIZE, 0, IPC_NOWAIT) == -1) {
    perror("msgrcv");
    exit(1);
  }
#endif

  printf("msg.mtext = %s\n", msg.mtext);

  return 0;
}

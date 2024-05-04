/*
 * author: inkXu
 * date:   2022.8.27
 *
 * 消息队列的控制
 *
 * int msgctl(int msqid, int cmd, struct msqid_ds *buf);
 * 具体用法参考man msgctl，这里只用了一点
 * msqid：消息队列的标识msqid，使用ipcs -q可以查看到，该表中有一列是msqid
 * cmd：函数功能的控制
        IPC_RMID：删除由msqid指示的消息队列，将它们从系统中删除并破坏相关数据结构
        IPC_STAT：将msqid相关的数据结构中各个元素的当前值存入由buf指向的结构中
        IPC_SET：将msqid相关的数据结构中的元素设置为由部分指向的结构中的对应值
 * buf：msqid_ds数据类型的地址，用来存放或更改消息队列的属性
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }

  system("ipcs -q");

  int msqid;
  if ((msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
    perror("msgget");
    exit(1);
  }

  system("ipcs -q");

  if (msgctl(msqid, IPC_RMID, NULL) == -1) {
    perror("msgctl");
    exit(1);
  }

  system("ipcs -q");

  return 0;
}

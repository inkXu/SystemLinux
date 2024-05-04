/*
 * author: inkXu
 * date:   2022.8.27
 *
 * 消息队列的创建
 *
 * int msgget(key_t key, int msgflg);
 * 第一个参数是key值，可以自己随便设置，但是不建议自己随便设置，容易重复，所以使用ftok函数来生成一个key值
 * msgflg取值：IPC_CREAT(创建消息队列)  IPC_EXCL(检测消息队列是否存在)
               位或权限位：设置消息队列的访问权限，和函数open的mode_t一样，但可执行权限未使用
 * msgflg可以写(IPC_CREAT | IPC_EXCL | 0666) 或 (IPC_CREAT | 0666)，第一个表示创建一个消息队列，若是有相同
   的消息队列存在，errno会被置为EEXIST，0666表示权限，也可以写成0777，第二种写法没有IPC_EXCL，所以不进行检
   测是否有同样的消息队列存在，不会将errno置为EEXIST，若存在则直接使用
 * 成功返回消息队列的标识符，失败返回-1
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
  key_t key;

  //可以用来调用shell系统命令
  system("ipcs -q");

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }
  printf("key = %#x\n", key);

  int msqid = 0;

#if 0
  //有IPC_EXCL检测是否已经存在key相同的消息队列时，若是键值为key的消息队列存在，msgget返回值为-1
  if ((msqid = msgget(key, IPC_CREAT | IPC_EXCL | 0777)) == -1) {
    perror("msgget");
    exit(1);
  }
#endif

#if 1
  //不进行判断是否存在有key相同的消息队列，若是存在，msgget直接返回已存在的消息队列
  if ((msqid = msgget(key, IPC_CREAT | 0666)) == -1) {
    perror("msgget");
    exit(1);
  }
  printf("msqid = %d\n", msqid);
#endif

  system("ipcs -q");

  return 0;
}

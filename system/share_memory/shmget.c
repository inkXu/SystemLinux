/*
 * author: inkXu
 * date:   2022.8.27
 *
 * 获得一个共享存储标识符
 *
 * int shmget(key_t key, size_t size, int shmflg);
 *
 * 第一个参数是key值，可以自己随便设置，但是不建议自己随便设置，容易重复，所以使用ftok函数来生成一个key值
 * size为申请的共享存储区大小，有上限，具体是多少不知道，最少也要写1个字节
 * shmflg取值：IPC_CREAT(创建消息队列)  IPC_EXCL(检测消息队列是否存在)
               位或权限位：设置消息队列的访问权限，和函数open的mode_t一样，但可执行权限未使用
               shmflg可以写(IPC_CREAT | IPC_EXCL | 0666) 或 (IPC_CREAT | 0666)
 * 成功返回一个共享存储区的标识，失败返回-1
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }

  int shmid;
  if ((shmid = shmget(key, 50, IPC_CREAT | 0666)) == -1) {
    perror("shmget");
    exit(1);
  }
  printf("shmid = %d\n", shmid);

  return 0;
}

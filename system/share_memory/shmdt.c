/*
 * author: inkXu
 * date:   2022.8.28
 *
 * 共享内存解除映射(detach)
 * 和shmat.c文件一起使用，这个程序是读取共享内存数据，shmat.c是写入数据
 *
 * int shmdt(const void *shmaddr);
 *
 * 功能：
 * 解除映射
 * 参数：
 * shmaddr：映射的地址，传入映射的地址
 * 返回值：
 * 成功返回0，失败返回-1
 *
 * shmat函数使用的时候第二和第三个参数一般设置为NULL和0，让系统自动指定共享内存地址，并且共享内存可读可写
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  int age;
  char name[16];
} Person;

int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }

  int shmid;

  if ((shmid = shmget(key, 128, IPC_CREAT | 0664)) == -1) {
    perror("shmget");
    exit(1);
  }

#if 1
  Person *text = NULL;

  if ((text = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    exit(1);
  }

  printf("text->age = %d, text->name = %s\n", text->age, text->name);
  printf("text->age = %d, text->name = %s\n", (text + 1)->age, (text + 1)->name);
#endif

#if 0
  char *text = NULL;

  if ((text = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    exit(1);
  }

  printf("text = %s\n", text);
#endif

  if (shmdt(text) == -1) {
    perror("shmdt");
    exit(1);
  }

  return 0;
}












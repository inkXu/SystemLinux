/*
 * author: inkXu
 * date:   2022.8.28
 *
 * 共享内存映射(attach)
 * 和shmdt.c文件一起使用，这个程序是向共享内存写入数据，shmdt.c是读取数据
 *
 * void *shmat(int shmid, const void *shmaddr, int shmflg);
 *
 * 功能：
 * 将一个共享内存段映射到调用进程的数据段中
 *
 * 参数：
 * shmid：共享内存的id(标识符)
 * shmaddr：映射的地址，设置为NULL为系统自动分配，推荐使用NULL
 * shflg：标志位，共享内存访问权限和映射条件，为0时表示共享内存具有可读可写权限，为SHM_RDONLY为只读权限，
          SHM_RND(shmaddr非空时才有效)
          没有指定SHM_RND则此段连接到shmaddr所指的地址上(shmaddr必须页对齐)
          指定了SHM_RND则此段连接到shmaddr - shmaddr%SHMLBA所表示的地址上
 *
 * 返回值：
 * 成功返回映射的地址，失败返回-1
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

void test1();
void test2();

int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(1);
  }

  int shmid;

  if ((shmid = shmget(key, 128, IPC_CREAT | 0666)) == -1) {
    perror("shimd");
    exit(1);
  }

#if 1
  Person *text = NULL;

  if ((text = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    exit(1);
  }

  text->age = 18;
  strcpy(text->name, "kiana");
  (text + 1)->age = 19;
  strcpy((text + 1)->name, "teriri");
#endif

#if 0
  //向共享内存中写入字符串
  char *text = NULL;

  //获取共享内存的映射
  //因为shmat返回的是一个void*，失败返回-1，-1不是一个指针，所以为了防止出问题，要将-1强制转换为void*类型
  if ((text = shmat(shmid, NULL, 0)) == (void *)-1) {
    perror("shmat");
    exit(1);
  }

  //strcpy(text, "welcome to china");
  strcat(text, "! thank you!");
#endif

  //解除共享内存映射
  if (shmdt(text) == -1) {
    perror("shmdt");
    exit(1);
  }

  return 0;
}











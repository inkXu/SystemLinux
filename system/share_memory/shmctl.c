/*
 * author: inkXu
 * date:   2022.8.28
 *
 * 共享内存的控制
 *
 * int shmctl(int shmid, int cmd, struct shmid_ds *buf);
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main()
{
  key_t key;

  if ((key = ftok(".", 100)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  int shmid;
  if ((shmid = shmget(key, 128, IPC_CREAT | 0666)) == -1) {
    perror("shmget");
    exit(1);
  }

  system("ipcs -m");

  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl");
    exit(1);
  }

  system("ipcs -m");

  return 0;
}

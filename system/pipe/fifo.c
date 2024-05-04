/*
 * author: inkXu
 * date:   2022.8.25
 *
 * 有名管道FIFO的创建
 *
 * int mkfifo(const char *pathname, mode_t mode);
 * 创建一个有名管道，产生一个本地文件系统可见的文件pathname
 * pathname有名管道创建的文件，可以带路径
 * mode管道文件的权限，一般通过八进制数设置即可
 * 成功返回0，失败-1
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
  if (mkfifo("./myfifo", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}

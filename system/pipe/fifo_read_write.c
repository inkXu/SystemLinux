/*
 * author: inkXu
 * date:   2022.8.26
 *
 * 有名管道实现进程间读写操作
 *
 * 和无名管道差不多，无名管道通过pipe获取文件描述符，而有名管道在文件系统中创建了一个文件，
 * 所以直接使用open打开管道文件
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  if (mkfifo("./wrfifo", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }

  int fd;
  //操作只需要写O_RDWR即可，不需要清空，因为其它进程可能使用该管道文件，也不需要创建，因为使用open创建的文件
  //是普通文件
  if ((fd = open("./wrfifo", O_RDWR)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if (write(fd, "welcome to china", sizeof("welcome to china")) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  char buf[32] = "";
  if (read(fd, buf, sizeof(buf)) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  printf("buf = %s\n", buf);

  close(fd);

  return 0;
}

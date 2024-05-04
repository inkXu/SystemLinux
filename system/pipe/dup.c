/*
 * author: inkXu
 * date:   2022.8.25
 *
 * 文件描述符的复制
 *
 * dup会复制一次参数内的文件描述符，复制的后新的文件描述符为最小可用的文件描述符
 * 成功返回新的文件描述符，失败返回-1
 * 等价于fcntl(int oldfd, O_DUPFD, 0);
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
#if 1
  //通过dup实现输出重定向，并恢复输出到终端
  
  //复制标准输出的文件描述符，out_fd=3
  int out_fd;
  if ((out_fd = dup(1)) == -1) {
    perror("dup");
    exit(1);
  }

  /* 不知为啥，一定要输出一句话才能实现输出重定向 */
  printf("out_fd = %d\n", out_fd);

  //fd = 4
  int fd;
  if ((fd = open("./test.txt", O_RDWR | O_TRUNC | O_CREAT, 0664)) == -1) {
    perror("open");
    exit(1);
  }

  //关闭标准输出文件描述符
  close(1);

  //fd1 = 1，实现重定向输出
  int fd1;
  if ((fd1 = dup(fd)) == -1) {
    perror("dup");
    exit(1);
  }

  //重定向输出到test.txt文件中
  printf("I love you\n");
  printf("I love you\n");

  //关闭fd1，就是printf使用的文件描述符
  close(fd1);

  //将标准输出进行复制，这时标准输出文件描述符又是1了
  if (dup(out_fd) == -1) {
    perror("dup");
    exit(1);
  }

  //向终端打印
  printf("welcome to china\n");
#endif

#if 0
  //通过dup实现重定向输出
  int fd = 0;
  if ((fd = open("./test.txt", O_CREAT | O_RDWR | O_TRUNC, 0664)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  printf("fd = %d\n", fd);

  //printf默认调用的文件描述符是1，所以只需要修改了文件描述符1所指的文件，即可实现输出重定向
  close(1);

  //实现输出重定向
  int fd1 = 0;
  if( (fd1 = dup(fd)) == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }

  printf("thank you for you love\n");

  close(fd);
  close(fd1);
#endif

  return 0;
}

/*
 * author: inkXu
 * date:   2022.8.25
 *
 * 文件描述符的复制2
 * 
 * dup2(int oldfd, int newfd);
 * oldfd表示你要复制的文件描述符，newfd表示你复制后想使用的文件描述符，newfd标识的文件和oldfd标识的文件一样
 * 注意：使用dup2复制文件描述符时，需要newfd有初始值，若是没有，newfd会被初始化为0，那么复制后标准输入的
   文件描述符将丢失，所以一定要给newfd赋初值，并且赋初值时不要占用了已经开启的文件描述符
 * 等价于close(newfd); fcntl(int oldfd, O_DUPFD, int newfd);
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
#if 1
  //使用dup2实现重定向，并复原标准输出使用的文件描述符
  int outfd = 3;
  //关闭了文件描述符1，将标准输出的文件描述符指定为3
  if (dup2(1, outfd) == -1) {
    perror("dup2");
    exit(1);
  }
  //和dup一样，不知道为什么要加了一次输出才有用实现输出重定向
  printf("outfd = %d\n", outfd);

  int fd;
  if ((fd = open("./test.txt", O_RDWR | O_CREAT | O_TRUNC, 0664)) == -1) {
    perror("open");
    exit(1);
  }

  if (dup2(fd, 1) == -1) {
    perror("dup2");
    exit(1);
  }
  printf("this is my program\n");

  if (dup2(outfd, 1) == -1) {
    perror("dup2");
    exit(1);
  }
  printf("welcome to china\n");
#endif
#if 0
  //通过dup2实现输出重定向
  int fd;
  if ((fd = open("./test.txt", O_RDWR | O_CREAT | O_TRUNC, 0664)) == -1) {
    perror("open");
    exit(1);
  }
  if (dup2(fd, 1) == -1) {
    perror("dup2");
    exit(1);
  }
  printf("thank you\n");
#endif

  return 0;
}

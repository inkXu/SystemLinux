/*
 * author: inkXu
 * date:   2022.8.26
 *
 * 有名管道非阻塞
 *
 * 有名管道设置非阻塞可以直接再open的打开方式直接设置，不需要像无名管道那样使用fcntl函数，其实fcntl函数
 * 功能挺丰富的，可以设置非阻塞和阻塞，还可以当dup和dup2的功能(duplicate file descriptor)
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
#if 0
  //以只读方式打开文件，并将该文件描述符设置为非阻塞类型，open函数将不会阻塞程序运行，后面read函数
  //返回值为0
  if (mkfifo("./non_block", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(1);
    }
  }
  printf("/*     this is a open function     */\n");
  int fd;
  if ((fd = open("./non_block", O_RDONLY | O_NONBLOCK)) == -1) {
    perror("open");
    exit(1);
  }
  printf("/*     this is a read function     */\n");
  char buf[1024] = "";
  if (read(fd, buf, sizeof(buf)) == -1) {
    perror("read");
    exit(1);
  }
  printf("buf = %s\n", buf);
#endif

#if 1
  //以只写方式打开文件，并将该文件描述符设置为非阻塞类型，open函数直接输出错误信息:no such device or address
  if (mkfifo("./non_block", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(1);
    }
  }
  printf("/*     this is a open function     */\n");
  int fd;
  if ((fd = open("./non_block", O_WRONLY | O_NONBLOCK)) == -1) {
    perror("open");
    exit(1);
  }
  printf("/*     this is a write function     */\n");
  char buf[1024] = "";
  if (write(fd, buf, sizeof(buf)) == -1) {
    perror("open");
    exit(1);
  }
#endif

  return 0;
}

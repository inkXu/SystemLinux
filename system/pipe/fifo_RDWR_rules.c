/*
 * author: inkXu
 * date:   2022.8.26
 *
 * 有名管道的读写规律
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
#if 1
                         /*    例子在前面学的无名管道通信   */
  //有名管道分别被两个不相关进程以只读和只写方式打开时，当只有一个进程开启时，阻塞在open函数，当另外一个
  //进程也开启时，open函数不再阻塞。
  //若是两个进程内都使用了循环来读取管道和写入管道时，若写入管道的那个进程先结束运行，那么另一个进程的
  //read函数返回值为0，若是循环内没有对这种情况进行判断，那么读取管道的程序会陷入死循环。若是读取管道的
  //进程先结束，那么另一个进程的write函数进行调用时马上就会产生一个SIGPIPE（管道破裂）信号，该信号默认
  //处理方式是结束进程运行。
#endif
#if 0
  //有名管道读写端都存在，一直读
  //若管道中没有了数据，read会发生阻塞
  if (mkfifo("./read", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }
  int fd;
  if ((fd = open("./read", O_RDWR)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  if (write(fd, "thank you", sizeof("thank you")) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  while(1) {
    char buf[64] = "";
    if (read(fd, buf, sizeof(buf)) == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    printf("buf = %s\n", buf);
  }
#endif

#if 0
  //有名管道读写端都存在，一直写
  //有名管道和无名管道一样，最大存放64k的数据，达到64kwrite发生阻塞
  if (mkfifo("./write", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }
  int fd;
  if ((fd = open("./write", O_RDWR)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  int i = 0;
  while(1) {
    if (write(fd, "thank you", 1024) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    printf("i = %d\n", ++i);
  }
#endif

#if 0
  //有名管道只有读端，若是有名管道文件以只读方式打开，open函数会发生阻塞
  if (mkfifo("./read", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }
  printf("/*       this is open function       */\n");
  int fd;
  //和无名管道有些不同，无名管道是使用pipe直接返回读写的文件描述符，此基础上关闭一个描述符来达到只读只写的
  //状态，而有名管道是使用open函数，直接指明是只读还是只写操作
  if ((fd = open("./read", O_RDONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  printf("/*      this is a loop of read function      */\n");
  while(1) {
    char buf[64] = "";
    if (read(fd, buf, sizeof(buf)) == -1) {
      perror("read");
      exit(1);
    }
    printf("buf = %s\n", buf);
  }
#endif

#if 0
  //有名管道只有写端，若是有名管道文件以只写方式打开，open函数会发生阻塞
  if (mkfifo("./write", 0664) == -1) {
    if (errno != EEXIST) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }
  printf("/*       this is open function       */\n");
  int fd;
  //和无名管道有些不同，无名管道是使用pipe直接返回读写的文件描述符，此基础上关闭一个描述符来达到只读只写的
  //状态，而有名管道是使用open函数，直接指明是只读还是只写操作
  if ((fd = open("./write", O_WRONLY)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  printf("/*      this is a loop of write function      */\n");
  while(1) {
    int i = 0;
    char buf[64] = "thank you for you love";
    if (write(fd, buf, sizeof(buf)) == -1) {
      perror("read");
      exit(1);
    }
    printf("i = %d\n", ++i);
  }
#endif

  return 0;
}

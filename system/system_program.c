/*
 * author: inkXu
 * date:   2022.8.16
 * 
 * 系统调用
 *
 * 可以通过命令man 2 open查看到open的详细参数即返回值
 * int open(const char *pathname, int flag, mode_t mode);
 * 其中的flag参数有以下几种
 * O_RDONLY(only read)     O_WRONLY(only write)    O_RDWR(read and write)
 * O_CREAT(不存在就创建)   O_EXCL(若文件存在还指定O_CREAT则报错)
 * O_TRUNC(如果文件存在则清空)  O_APPEND(向文件追加内容)
 * O_NONBLOCK(打开的是管道文件fifo、字符文件、块文件，此选项为非阻塞标志位)
 * mode为权限，权限使用的是8进制数，为创建一个文件时该文件的权限
 * 返回值为文件描述符，若是失败返回-1
 *
 * 注意：内核的系统调用没有缓冲区一说，所以每次调用时都会直接对磁盘空间进行写入
 */
// perror接口所在的头文件
#include <stdio.h>
// errno变量所在的头文件
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// wirte、close接口所在的头文件
#include <unistd.h>
#include <string.h>

void test1();    //一个程序最大开启的文件个数
void test2();    //测试open、write、perror、errno
void test3();    //文件描述符的创建规则
void test4();    //read
void test5();    //remove

int main(int argc, char *argv[])
{
  test4();

  return 0;
}

void test5()
{
  if( remove("./file.txt") == -1 )
  {
    perror("fail to remove");
    return ;
  }
  write(1, "delete success\n", 15);
}

void test4()
{
  int fp = 0;
  if( (fp = open("./file.txt", O_RDONLY)) == -1 )
  {
    perror("fail to open");
    return ;
  }
  char buf[32] = "";
  ssize_t bytes;
  while(1)
  {
    if( (bytes = read(fp, buf, 12)) == -1 )
    {
      perror("fail to read");
      return ;
    }
    if( bytes == 0 )
    {
      break;
    }
    printf("%s", buf);
    memset(buf, 0, sizeof(buf));
  }
  printf("\n");
}

void test3()
{
  //从小到大依次创建
  int fp = open("./file.txt", O_CREAT, 0664);
  printf("fp = %d\n", fp);
  int fp1 = open("./file.txt", O_CREAT, 0664);
  printf("fp1 = %d\n", fp1);
  close(fp);
  int fp2 = open("./file.txt", O_CREAT, 0664);
  printf("fp2 = %d\n", fp2);
  int fp3 = open("./file.txt", O_CREAT, 0664);
  printf("fp3 = %d\n", fp3);

  close(fp);
  close(fp1);
  close(fp2);
  close(fp3);
}

void test2()
{
#if 1
  int fp = open("./file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if( fp == -1 )
  {
    //errno为错误码，为整型，可以在/usr/include/asm-generic/errno-base.h文件中查找错误信息
    printf("errno = %d\n", errno);
    perror("fail of open");
    return ;
  }
  //第一个参数为文件描述符，第二个为写入的字符串
  //第三个为写入的字符数，要写多少就写多少，若是数字大了，会写入字符串后面地址的内容
  //返回值为写入字符数，发生错误时返回-1，返回类型为ssize_t，为长整型
  ssize_t bytes;
  if( (bytes = write(fp, "welcome to china\n", 17)) == -1 )
  {
    perror("fail to write");
    return ;
  }
  printf("bytes = %ld\n", bytes);
  //在终端输出值
  write(1, "I love you\n", 11);
  close(fp);
#endif
}

void test1()
{
  //在一个程序中最多开启1024个文件，0-1023，包括了标准输入、输出和错误
  int i = 0, fp = 0;
  for( ; i < 1025; i++)
  {
    if( (fp = open("./file.txt", O_RDONLY | O_CREAT, 0664)) == -1 )
    {
      perror("fail to open");
      return ;
    }
    printf("fp = %d\n", fp);
  }
  close(fp);
}






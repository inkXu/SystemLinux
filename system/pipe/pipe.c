/*
 * author: inkXu
 * date:   2022.8.24
 *
 * 无名管道的创建
 *
 * int pipe(int fd_pipe[2])
 * 参数为一个数组指针，返回了无名管道的文件描述符到数组中，fd_pipe[0]为读操作，fd_pipe[1]为写操作，
 * 成功返回值为0，失败返回值为-1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  //无名管道的创建
  typedef ARR[2];
  ARR pipefd;
  if (pipe(pipefd) == -1) {
    perror("fail to pipe");
    exit(EXIT_FAILURE);
  }

  //无名管道的写操作，注意，数组下标为1才是写操作
  if (write(pipefd[1], "thank you for you love", sizeof("thank you for you love")) == -1) {
    perror("fail to write");
    exit(EXIT_FAILURE);
  }

  //无名管道读操作，数组下标为0
  ssize_t bytes = 0;
  char buf[16] = "";
  if ((bytes = read(pipefd[0], buf, sizeof(buf))) == -1) {
    perror("fail to read");
    exit(EXIT_FAILURE);
  }

  printf("bytes = %d\n", bytes);
  printf("buf = %s\n", buf);

  if ((bytes = read(pipefd[0], buf, sizeof(buf))) == -1) {
    perror("fail to read");
    exit(EXIT_FAILURE);
  }

  printf("bytes = %d\n", bytes);
  printf("buf = %s\n", buf);

  return 0;
}

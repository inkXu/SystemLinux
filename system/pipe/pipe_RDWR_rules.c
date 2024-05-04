/*
 * author: inkXu
 * date:   2022.8.24
 *
 * 默认用read读取无名管道中的数据时，若管道中没有数据时，read是阻塞的
 * 调用write函数向管道中写数据，当缓冲区满了时write也会阻塞，无名管道没有数据也会阻塞，缓冲区为64k个字节
 * 通信过程中，读端口关闭后，写进程向管道内写数据时，写进程会收到SIGPIPE信号，从而退出进程
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main()
{
  int pipefd[2];

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

//读写端存在，一直写，写够了64k后管道满了，会等待读端读取数据，write发生阻塞
#if 0
  int num = 0;
  while(1) {
    if (write(pipefd[1], "yes", 1024) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    printf("num = %d\n", num++);
  }
#endif

//读写端存在，一直读，当管道中没有数据后，read会等待写端写入数据，read发生阻塞
#if 0
  if (write(pipefd[1], "yes", 3) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  char buf[32];
  memset(buf, 0, sizeof(buf));

  if (read(pipefd[0], buf, 3) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  printf("buf = %s\n", buf);

  memset(buf, 0, sizeof(buf));

  if (read(pipefd[0], buf, 3) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  printf("buf = %s\n", buf);
#endif

//只有读端，注意，若是管道内数据没了，read会返回0，不要使用while循环，不然要加个参数来判断一下返回值，
//出错时-1，读完0，读取成功返回读取的字节数
#if 0 
  if (write(pipefd[1], "yesaogiohagieoahoidioahgoihoiahgeoahgo", 50) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  //关闭写入端
  close(pipefd[1]);

  char buf[32];
  ssize_t bytes = 0;

  memset(buf, 0, sizeof(buf));
  if ((bytes = read(pipefd[0], buf, 30)) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  printf("buf = %s\n", buf);
  printf("bytes = %d\n", bytes);

  if ((bytes = read(pipefd[0], buf, 30)) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  printf("buf = %s\n", buf);
  printf("bytes = %ld\n", bytes);

  if ((bytes = read(pipefd[0], buf, 30)) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  printf("buf = %s\n", buf);
  printf("bytes = %ld\n", bytes);
#endif

//只有写端，若只有写端，程序第一次调用write时，会产生SIGPIPE信号，该信号的默认处理方式是停止该进程
#if 1
  //将SIGPIPE设置为默认处理方式
  void handler(int sig);
  signal(SIGPIPE, handler);

  //关闭管道的读操作
  close(pipefd[0]);

  while(1) {
    if (write(pipefd[1], "thank", 1024) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }
#endif

  return 0;
}

void handler(int sig)
{
  printf("+----------------------+\n");
  printf("|       SIGPIPE        |\n");
  printf("+----------------------+\n");
}

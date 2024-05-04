/*
 * author: inkXu
 * date:   2022.8.25
 *
 * 设置文件阻塞特性
 *
 * 阻塞：管道中没有数据，read会一直等待，知道有数据才会继续运行，否则一直等待
 * 非阻塞：read函数运行时，会先看一下管道中是否有数据，如果有数据，则正常读取数据，如果管道中没有数据，
   则read函数会立刻返回，继续下面的代码运行
 * 设置为阻塞特性：fcntl(fd, F_SETFL, 0);
 * 设置为非阻塞特性：fcntl(fd, F_SETFL, NONBLOCK);
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  int pipefd[2];

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int cpid = fork();

  if (cpid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (cpid > 0) {

    //将管道的读操作设置为非阻塞，把read的错误信息判断代码关了，不然会输出
    //错误提示：Resource temporarily unavailable(资源暂时不可用)
    fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    //将管道的读操作设置为阻塞状态，默认就是该状态，在该状态时，read会等待管道中加入数据，否则一直等待
    //fcntl(pipefd[0], F_SETFL, 0);

    while(1) {
      char buf[32] = "";
      read(pipefd[0], buf, sizeof(buf));
#if 0
      if (read(pipefd[0], buf, sizeof(buf)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
#endif
      sleep(1);
      printf("the child process message is: %s\n", buf);
    }
  }
  else {
    while(1) {
      char buf[] = "welcome to china";
      if (write(pipefd[1], buf, sizeof(buf)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
      sleep(3);
    }
  }

  return 0;
}

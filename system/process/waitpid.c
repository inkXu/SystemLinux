/*
 * author: inkXu
 * date:   2022.8.18
 *
 * 进程等待
 *
 * waitpid，man手册有英文版
 * 功能：等待子进程终止，如果子进程终止了，此函数会回收子进程的资源。
 *       调用wait函数的进程会挂起，直到它的一个子进程退出或收到一个不能被忽视的信号才被
 *       若调用进程没有子进程或它的子进程已经结束，该函数立马返回。
 * 参数：pid：用于指定某个进程
 *            pid > 0：等于进程ID等于pid的子进程。
 *            pid = 0：等待同一个进程组中的任何子进程，如果子进程加入了别的进程组，waitpid失效
 *            pid = -1：等待任一子进程，此时waitpid与wait作用一样
 *            pid < -1：等待指定进程组中任何子进程，这个进程组的ID等pid的绝对值
 *       status：函数返回时，参数status中包含子进程退出时的状态信息。
 *       子进程的退出信息在一个int中包含了多个字段，用宏定义可以取出其中的每个字段
 *       子进程可以通过exit或者_exit函数发送退出状态
 *       options：该参数中包含子进程退出时的状态信息
 *                0：同wait，阻塞父进程，等待子进程退出。
 *                WNOHANG：没有任何已经结束的子进程，则立即返回。
 *                WUNTRACED：如果子进程暂停了则此函数马上返回，并且不予以理会子进程的结束状态。(跟踪调试，少用)
 * 返回值：
 *       成功：返回状态改变了的子进程的进程号，如果设置了选项WNOHANG并且pid指定的进程存在则返回0，
 *       出错：返回-1，当pid所指示的子进程不存在，或此进程存在，但不是调用进程的子进程，waitpid就会出错返回，
 *       这时errno被设置为ECHILD。
 *
 * 取出子进程的退出信息
 *       WIFEXITED(status):
 *       如果子进程是正常终止的，取出的字段值非零。
 *       WEXITSTATUS(status):
 *       返回子进程的退出状态，退出保存在status变量的8-16位，在使用此宏之前应该先用
 *       WIFEXITED判断子进程是否正常退出，正常退出才能使用此宏
 *
 * 特殊进程：
 *       僵尸进程（Zombie Process）
 *       进程已运行结束，但进程占用的资源为被回收，这样的进程成为僵尸进程。
 *       子进程已运行结束，父进程为调用wait或waitpid函数回收子进程的资源是子进程变为僵尸进程的原因。
 *       孤儿进程（Orphan Process）
 *       父进程运行结束，但子进程没有运行结束，这样的子进程成为孤儿进程。
 *       守护进程（精灵进程）（Daemon Process）
 *       守护进程是一个特殊的孤儿进程，这种进程脱离终端，在后台运行。比如系统的进程
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fail ro fork");
    return -1;
  }
  else if(pid == 0)
  {
    int i = 0;
    for(; i < 5; i++)
    {
      printf("this is a child process\n");
      sleep(1);
    }
    //我这系统好像只有_exit函数可以在子进程中用来返回结束信息，exit和_exit不在同一个同文件中
    _exit(3);
  }
  else
  {
    pid_t status = 0;
    //第一个参数大概就是把哪个进程当作条件，看它或它们是否结束运行
    //第二个参数就是存放第一个参数所指定的进程的结束信息
    //第三个参数就是怎样阻塞，比如0就是要等子进程执行完了才会继续向下执行
    waitpid(-1, &status, 0);
    printf("this is a parent process\n");
    if(WIFEXITED(status) != 0)
    {
      printf("status information is %d\n", WEXITSTATUS(status));
    }
  }
  return 0;
}







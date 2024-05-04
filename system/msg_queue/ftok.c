/*
 * author: inkXu
 * date:   2022.8.27
 *
 * 消息队列的创建
 * 
 * key_t ftok(const char *pathname, int proj_id);
 * 可以直接生成key值，但是为了减小重复率，一般使用ftok来生成key值
 * 通过该函数可以生成唯一的key值，该key值是通过一个路径和一个项目ID(低八位 2^8=128)生成的，proj_id
 * 是一个非0整数，不能使用0，从1-128，若是超过这个范围，proj_id会只取该数组的低八位
 * 成功返回创建的key值，失败返回-1
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main()
{
  key_t key;
  //通过当前目录和110来生成一个key值，若是该程序一直在同一个目录下运行，且proj_id不改变，那么key值一定
  //相同，若是将该程序移动到其它目录，那么key值会改变
  if ((key = ftok(".", 128)) == -1) {
    perror("ftok");
    exit(1);
  }
  //使用16进制数输出，在x前的#表示16进制的前导符
  printf("key = %#x\n", key);

  if ((key = ftok(".", 128)) == -1) {
    perror("ftok");
    exit(1);
  }
  printf("key = %#x\n", key);

  return 0;
}

/*
 * author: inkXu
 * date:   2022.9.8
 *
 * 地址转换函数
 *
 * 使用地址转换主要是因为人为识别的ip地址是点分十进制的字符串形式，但是计算机或网络中识别的IP地址是整型
 * 数据，所以需要转换
 *
 * int inet_pton(int af, const char *src, void *dst);
 * af：协议族，只能写AF_INET(ipv4)或AF_INET6(ipv6)
 * src：点分十进制数串，就是我们能看懂的ip地址192.168.3.1
 * dst：32位无符号的整型数据，直接输出是一个整型数据，分别对这四个字节内存读取数据时是我们所识别的IP地址
 * 成功返回1，失败返回其它
 * 该函数ipv4和ipv6都可以转换
 *
 * const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
 * af：协议族，只能写AF_INET(ipv4)或AF_INET6(ipv6)
 * src：32位无符号的整型数据
 * dst：点分十进制数串，就是我们能看懂的ip地址192.168.3.1
 * size：dst缓冲区长度，dst宏定义
                       #define INET_ADDRSTRLEN 16   //for ipv4
                       #define INET6_ADDRSTRLEN 46  //for ipv6
 * 成功非空指针，失败返回NULL
 *
 * 下面几个函数和前面两个函数功能差不多，但是只能转换ipv4的地址，在UDP和TCP编程中会用的较多，传入的参数
 * 为一个结构体，该结构体中有点分十进制的IP地址的成员
 * int inet_aton(const char *cp, struct in_addr *inp);   //类似inet_pton
 * char *inet_ntoa(struct in_addr in);                   //类似inet_ntop
 * in_addr_t inet_addr(const char *cp);                  //类似inet_pton
 */
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

void test1();            //点分十进制IP地址转为机器能识别的32位整型数据
void test2();            //32位整型数据转为点分十进制IP地址

int main(int argc, char const *argv[])
{
  test2();

  return 0;
}

void test2() {
  //定义时是无符号char类型的数组，不是int类型的，相当于找一个连续的4字节的内存，每个字节存入一个整型数据
  unsigned char ip_int[] = { 10,0,18,235 };
  char ip_str[16] = "";
  if (inet_ntop(AF_INET, ip_int, ip_str, INET_ADDRSTRLEN) == NULL) {
    perror("inet_ntop");
    exit(EXIT_FAILURE);
  }
  printf("ip_str = %s\n", ip_str);
}

void test1() {
  char ip_str[] = "192.168.101.132";
  unsigned int ip_int = 0;

  if (inet_pton(AF_INET, ip_str, (void *)&ip_int) != 1) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  printf("ip_int = %#x\n", ip_int);

  //char为一个字节，所以使用char类型的指针来指向这个32位无符号的整型数据，char为一个字节，共8位，所以该
  //指针只需要+++就能读取到该32位无符号整型数据的全部内容
  unsigned char *ip_p = NULL;
  ip_p = (char *)&ip_int;

  printf("*ip_p = %d.%d.%d.%d\n", *ip_p, *(ip_p + 1), *(ip_p + 2), *(ip_p + 3));
}














/*
 * author: inkXu
 * date:   2022.9.8
 *
 * 判断当前系统的字节序
 */
#include <stdio.h>
#include <arpa/inet.h>

void test1();
void test2();

//共用体，共用同一块内存，共用体大小为共用体内变量占用字节数最大的变量的大小
//endian 字节序
union endian {
  char a;
  int e;
};

int main(int argc, char *argv[])
{
  test2();

  return 0;
}

//字节序的转换
void test2() {
  int a = 0x12345678;
  short b = 0x1234;

  printf("a = %#x\n", a);
  printf("b = %#x\n", b);

  //从主机字节序转换为网络字节序，转换32位长整型数值，返回值为uint32_t类型
  printf("htonl(a) = %#x\n", htonl(a));
  //从主机字节序转换为网络字节序，转换16位短整型数值，返回值为uint16_t类型
  printf("htons(b) = %#x\n", htons(b));
}

//判断当前系统的字节序
void test1() {
  //测试为小端存储
  union endian e;
  e.e = 0x12345678;

  printf("e.e = %#x\n", e.e);
  //e.a变量因为和e.e在一个共用体内，所以e.a获取的值为e.e变量的第一个低地址位
  //不能在共用体中多定义几个char类型变量来获取0x56和0x34等的值，因为共用一块地址，
  //多定义的char类型的变量使用时还是int类型的低地址的值
  printf("e.a = %#x\n", e.a);
}


# author: inkXu
# date:   2022.8.13

# shell函数

# 无参数函数
Print()
{
  echo "I love you"
  echo "welcome to china"
}
Print

# 参数函数
Add()
{
  # shell函数参数的传递通过$1,$2...来传递
  num1=$1
  num2=$2
  # 注意做相加操作时，需要写成下面这样，具体是啥还不知道，应该是一个字符串转整数相加的东西
  sum=`expr $num1 + $num2`
  echo "sum = $sum"
}
# 有参函数的调用，传入的参数写在函数名后面
Add 34 54

# 有参数和返回值的函数
Myadd()
{
  A=$1
  B=$2
  Sum=`expr $A + $B`
  return $Sum
}
Myadd 34 54
# 使用$?输出上一条命令的返回值，它是有限制的，$?最大值为255，所以不使用$?输出结果
# echo "Myadd 34 54 value is $?"
# 可以直接使用$Sum输出结果，因为在同一个shell中，定义的变量都是全局变量，都可以使用
# 除了()中定义的变量，只要不做任何修饰，就可以认为是全局变量
echo "Myadd 34 54 value is $Sum"












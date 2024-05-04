# author: inkXu
# date:   2022.8.11

# 脚本变量的特殊用法："" '' `` \ () {}

MYNUM=808

# ""：双引号里的内容特殊字符有特殊意义
echo -e "the variable MYNUM is $MYNUM"

# ''：单引号里的内容特殊字符没有特殊意义，单纯一个字符串
echo -e 'the variable MYNUM is $Num,\tthak you\n'

# \ ：和C的一样，但是使用echo输出时，要加入-e选项
echo -e "thak you.\n"

# ``: 反引号中的内容作为系统命令，并执行其内容，可以替换输出为一个变量
echo "Current date is `date`"

# ()：括号内的命令是通过子shell来执行的，执行过后的结果不会影响当前shell中的变量值等
echo "variable MYNUM is $MYNUM"
( MYNUM=10086;echo "variable MYNUM is $MYNUM" )
echo "variable MYNUM is $MYNUM"

# {}：括号内的命令是通过当前shell来执行的，执行过后的结果会影响当前shell中的变量值等
# 注意{ code }，要在{后加上一个空格，}前也要有一个空格，理论上是这样，我的{}在同一行不行
# 下面这个报错
# { MYNUM=10086;echo "variable MYNUM is $MYNUM" }
# 所以最好这样写
{
  MYNUM=10086
  echo "variable MYNUM is $MYNUM"
}
echo "variable MYNUM is $MYNUM"

# author: inkXu
# date:   2022.8.11

# 条件测试语句

# test命令：用于测试字符串、文件状态和数字
# test命令有两种格式：
#     test condition 或 [ condition ]，使用第二种方法时注意在条件两边加上空格
# shell脚本的条件测试有：文件测试、字符串测试、数字测试、复合测试
# 测试语句一般和条件语句联合使用

# 条件测试语句 文件测试(linux下的文件类型有bcd-lsp)
# condition 有三中情况：
# 文件类型
#    -e(exist,文件是否存在)，-s(是否为空)，-b(块设备文件)，-c(char,字符设备文件)，-d(directory,目录文件)，
#    -f(普通文件)，-L(注意大写，软链接文件)，-S(socket,注意大写，套接字文件)，-p(pipe,管道文件)
# 文件权限(为当前用户的权限)
#    -r(可读)，-w(可写)，-x(可执行)
# 两个文件之间的比较
# file1 -nt file2     文件1的修改时间是否比文件2新
# file1 -ot file2     文件1的修改时间是否比文件2旧
# file1 -ef file2     两个文件的i node节点号是否一样，用于判断是否是硬链接

echo "please enter a file name >>>"
read FILE

test -e $FILE
echo "file $FILE is exist? $?"

test -s $FILE
echo "file $FILE is not null? $?"

test -b $FILE
echo "file $FILE is block file? $?"

test -c $FILE
echo "file $FILE is character device file? $?"

# 新创建的目录也不为空，因为一个目录默认是有.和..
test -d $FILE
echo "file $FILE is directory? $?"

test -f $FILE
echo "file $FILE is a common file? $?"

test -L $FILE
echo "file $FILE is a soft_link file? $?"

test -S $FILE
echo "file $FILE is a socket file? $?"

test -p $FILE
echo "file $FILE is a pipeline file? $?"

[ -r $FILE ]
echo "file $FILE is readable? $?"
[ -w $FILE ]
echo "file $FILE is writable? $?"
[ -x $FILE ]
echo "file $FILE is executable? $?"

# 条件测试语句 字符串测试
# s1 = s2           测试两个字符串内容是否完全一样
# s1 != s2          测试两个字符串内容是否有差异
# -z(zero) s1       测试s1字符串长度是否为0
# -n s1             测试s1字符串长度是否不为0

echo "please enter two strings s1 and s2 >>>"
read s1 s2

test s1 = s2
echo "s1 is same as s2? $?"

test s1 != s2
echo "s1 is not same as s2? $?"

test -z s1
echo "s1's length is zero? $?"

test -n s1
echo "s1's length is not zero? $?"

# 条件测试语句 数字测试
# a -eq(equal) b
# a -ne(not equal) b
# a -gt(greater than) b
# a -ge(greater equal) b
# a -lt(less than) b
# a -le(less equal) b

echo "please enter two numbers a and b >>>"
read a b
[ $a -eq $b ]
echo "a is equal b? $?"
[ $a -ne $b ]
echo "a is not equal b? $?"
[ $a -lt $b ]
echo "a is less than b? $?"
[ $a -gt $b ]
echo "a is greater than b? $?"
[ $a -le $b ]
echo "a is less equal b? $?"
[ $a -ge $b ]
echo "a is greater equal b? $?"

# 条件测试语句 复合测试
# 命令执行  && , ||             多重条件判定   -a(&&) , -o(||) , !
# 区别在于命令执行在&&和||的左右都要加test的完整语句，多重判定不需要，但需要加上参数
# test 1 && test 1               test 1 -a test 1

echo "please enter a number >>>"
read num

test $num -ge 3 -a $num -le 10
echo "the num is greater equal 3, and less equal 10? $?"
test $num -ge 3 && test $num -le 10
echo "the num is greater equal 3, and less equal 10? $?"

echo "please enter a number >>>"
read two

test $two -lt 8 || test $two -gt 21
echo "ths number two is less than 8 and greater than 21? $?"
test $two -lt 8 -o $two -gt 21
echo "ths number two is less than 8 and greater than 21? $?"

test ! -e condition_file.sh
echo "file condition_file.sh is not exist? $?"









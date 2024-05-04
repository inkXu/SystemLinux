
# author: inkXu
# date:   2022.8.13

# 控制语句

# if语句
# if后接的是[  ]条件判断语句，if和elif后都要加上than，同一行需要在条件后加;，在下一行不用，结束if时要加fi
echo "please enter a number"
read num

if [ $num -gt 30 ]
then
  echo "this number is greater than 30"
elif [ $num -le 30 ]; then
    if [ $num -eq 30 ]
    then
      echo "this number is equal 30"
    else
      echo "this number is less than 30"
    fi
else
  echo "what you entered is not a number"
fi

# case语句
# 和C中的switch类似，但是不能和C一样，在一个条件下不写;;结束语句
case "$1" in             # switch($1)
  "one")                 # 等同于C中的case "one":
  echo "one"
  ;;                     # 等同于C中的break
  "two")
  echo "two"
  ;;
  *)                     # 在C中等同于default:
  echo "other result"
  ;;
esac                     # case条件结束要加esac，case倒着写

# 多种情况对应相同的输出
echo "pleas input your choice"
read flag
case "$flag" in
  Y | y | yes | Yes | YES)
    echo "It's  right"
    ;;
  n* | N*)               # *为通配符
    echo "It's wrong"
    ;;
  *)
    exit 1               # 退出当前执行的脚本，若后面还有语句未执行，也不再执行
esac

# for循环，有两种形式
# 第一种
# 用于声明sum变量是一个整数变量，只能保存整数
declare -i sum
for((i=0; i<=100; i++))
do
  # 不要使用空格隔开
  sum=sum+i
done
echo "sum = $sum"

# 第二种
for num in 1 2 3 5 6
do
  echo "num = $num"
done

# 第二种方法实用例子，循环命令，将每次显示的文件名放入file变量中
for file in `ls`
do
  if [ -f $file ]
  then
    echo "$file is common file"
  elif [ -d $file ]
  then
    echo "$file is directory"
  else
    echo "^_^"
  fi
done

# while循环，condition不成立时退出循环
# 声明整型变量i和sum
declare -i i
declare -i sum
while [ "$i" != "101" ]
do
  sum=sum+i
  i=i+1
done
echo "sum = $sum"

# until循环与while循环相反，condition成立时退出循环
declare -i ii
declare -i ss
# until 直到condition时
until [ "$ii" = "101" ]
do
  ss=ss+ii
  ii=ii+1
done
echo "ss = $ss"

# break和continue
# 和C语言的一样
for v in 1 2 3 4 5 6 7 8
do
  if [ "$v" = "5" ]; then
    break
  else
    echo "$v"
  fi
done

declare -i bb
while [ "$bb" != "10" ]
do
  bb=bb+1
  if [ "$bb" = "6" ]
  then
    continue
  else
    echo "$bb"
  fi
done










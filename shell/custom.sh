# author: inkXu
# date:   2022.8.11

# 自定义变量

#!/bin/bash

ls

pwd

# 变量赋值操作时，等号两边不能有空格
NUM=808

# 调用变量名时，要使用$符
echo NUM = $NUM

# 从终端获取值，类型不限，可以数字，也可字符串
read _read
echo _read = $_read

# 用于消除变量，我记得linux有个啥变量表，系统自带了一些变量，其值给好了，若是没有消除变量，那么好像
# 会一直在这个表中
unset NUM
unset _read

# 定义个只读变量，不能unset
# readonly _readonly=999

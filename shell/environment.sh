# author: inkXu
# date:   2022.8.11

# 环境变量的定义

# 使用系统命令cnv可以参看所有的环境变量

# 先定义一个普通变量，环境变量在任何sh文件都能访问，传统上，定义时变量名全部大写，现在这样写或是
# 在命令行上写都只是临时的环境变量，若是想永久，需要写入文件，这两个文件路径在~/.bashrc或/etc/profile
# 这两个文件是系统自动调用的脚本，每次开机会执行吧，有的环境变量是会发生更改的，如HOME和PWD等
# 若是在配置文件中定义，需要重启系统或是重开一个终端或是使用source命令更改过的文件~/.bashrc才能生效
# 不知到是不是系统问题，Centos 家目录下没有.bashrc文件

# 需要在命令行敲才能生效，直接执行该shell可能作用域只在该文件吧
MYVAL=9999
export MYVAL

echo "MYVAL = $MYVAL"

echo "your home dirctory is $HOME"

echo "Current work dirctory is $PWD"

echo "Your shell is $SHELL"

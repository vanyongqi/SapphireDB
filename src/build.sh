#!/bin/sh

# autoscan     1
#生成configure.scan 
#mv configure.scan configure.in   1

#改名为configure.in

#设置AC_INIT
#AC_USE-SYSTEM_EXTENSIONS
#AM_INIT_AUTOMAKE
#AM_CONFIG_SRCDIR

aclocal
autoconf
#生成configure脚本
autoheader
#生成configure.h.in
# vim Makefile.am 1
#建立makefile
automake-add-missing

./configure CXXFLAGS= CFLAGS=

make

#1 的标记 是第一次编译 需要手动设置相关变量
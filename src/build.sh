#!/bin/sh

autoscan
#生成configure.scan 
mv configure.scan configure.in
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
vim Makefile.am
#建立makefile
automake-add-missing

./configure CXXFLAGS=CFLAGS=

make


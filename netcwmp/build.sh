#!/bin/bash
PRJ_PATH=`pwd`
#CROSS=/opt/buildroot-2011.11/arm920t/usr/bin/arm-linux-
#CROSS=/opt/buildroot-2012.08/arm920t/usr/bin/arm-linux-
#CROSS=/home/xueqian/ctools/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin/arm-linux-gnueabihf-
#make clean
#autoreconf -i
export CC=${CROSS}gcc
export CPP=${CROSS}cpp
export AS=${CROSS}as
export LD=${CROSS}ld
export AR=${CROSS}ar
export RANLIB=${CROSS}ranlib
export STRIP=${CROSS}strip
#export ac_cv_func_malloc_0_nonnull=yes
#export ac_cv_func_realloc_0_nonnull=yes
#export ac_cv_have_decl_isinf=yes
#export ac_cv_have_decl_isnan=yes
#export LDFLAGS += -lm
#./configure  --build=i686-pc-linux-gnu --host=arm-linux --prefix=/home/xueqian/opt/cwmp
./configure  --build=i686-pc-linux-gnu  --prefix=/home/xueqian/opt/cwmp 

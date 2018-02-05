#!/bin/bash
re_dir=root@192.168.1.136:/opt/init/
OBJ=state-detection


arm-linux-gnueabihf-gcc ./*.c -o $OBJ -lpthread
cp $OBJ ~/project/cpe/opt/init/ 
scp $OBJ $re_dir

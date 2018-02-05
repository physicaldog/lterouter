#!/bin/bash
opt=~/project/cpe/opt
web=~/ubuntuShare/web


cd websvr-goahead-2.18/LINUX/
make

# cp a6g2c-webs $web/webroot/bin

cd $web
cp -r $web/webroot/* $opt/web/

scp -r  $opt/web root@192.168.1.88:/opt/

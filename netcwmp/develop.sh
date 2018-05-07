#!/bin/sh

#./configure --target=arm-mv5sft-linux --host=arm-mv5sft-linux CC=arm-mv5sft-linux-gnueabi-gcc
#make clean && make
make
#scp cwmpd/src/cwmpd root@192.168.4.200:/bin/cwmpd
./expect_scp.sh 192.168.4.200 root ebang_20150601 cwmpd/src/cwmpd /bin/cwmpd
./expect_scp.sh 192.168.4.200 root ebang_20150601 conf/cwmp.conf /etc/cwmp.conf
./expect_scp.sh 192.168.4.200 root ebang_20150601 conf/device.xml /etc/device.xml



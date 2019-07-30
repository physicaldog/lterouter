#!/bin/bash
ret=`pidof Reboot.sh`
echo `date +%Y%m%d-%H:%M:%S` $0 "pid:" $ret $* >> /opt/log/syslog
mv /var/log/message* /opt/log/
sync;

while [ 1 ]
do
	echo `uptime` "reboot Device" >> /opt/log/syslog
	reboot 
	sleep 10 
	echo `uptime` "reboot -f Device" >> /opt/log/syslog
	reboot -f
done

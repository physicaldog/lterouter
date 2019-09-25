#!/bin/bash
ret=`pidof Reboot.sh`
echo `date +%Y%m%d-%H:%M:%S` $0 "pid:" $ret $* >> /opt/web/webPage/log/syslog
#mv /var/log/message* /opt/log/
mv /var/log/message* /opt/web/webPage/log/
sync;

while [ 1 ]
do
	echo `uptime` "reboot Device" >> /opt/web/webPage/log/syslog
	reboot 
	sleep 10 
	echo `uptime` "reboot -f Device" >> /opt/web/webPage/log/syslog
	reboot -f
done

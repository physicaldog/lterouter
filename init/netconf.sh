#!/bin/bash
. /opt/init/common.sh

log_syslog "netconf.sh start run!"
ret=`pidof udhcpc`
kill -9 $ret

udhcpc -b -i usb0 -p /opt/tmp/udhcpc.pid -R

ret=`ifconfig usb0 | grep "inet addr"`
if [ $? -eq 0 ]
then
	ret=`ifconfig usb0|grep inet|grep -v inet6|awk '{print $2}'|tr -d "addr:"`
	log_syslog "成功获取IP:" $ret
	uci -c /opt/config set status.system.wan_ip=$ret
	uci -c /opt/config commit status.system.wani_ip
    echo 1 > /proc/sys/net/ipv4/ip_forward
    route add default dev usb0
    #iptables -t nat -A POSTROUTING -o usb0 -j MASQUERADE
	#ptables -t nat -A POSTROUTING -s 192.168.10.0/24 -j SNAT --to 90.4.128.188
	#iptables -t nat -A PREROUTING -d 90.4.128.188 -j DNAT --to 192.168.10.15
	sleep 20;
else
	log_syslog "获取IP失败！"
fi

#!/bin/bash
#每分钟循环测试到主站的PING包丢包率，丢包率高于15%时断开以太网
#
ret=0
count=0
done=0
notdone=0;
Logfile="/opt/web/webPage/log/syslog"
/opt/dtu/open 0
while [ 1 ]
do
	echo `date +%Y-%m-%d-%H:%M:%S` "ping:" $1 $2 "packet" >> $Logfile
	ret=`ping -c $2 -W 2 $1 |awk -F, '//{for(i=1;i<=NF;i++) if($i ~ /loss/) print $i}' |cut -d'%' -f1`
	#if [ -z $ret ]
	#then
	#	echo `date +%Y-%m-%d-%H:%M:%S` "net bad" $ret >> $Logfile
	#	sleep 10
	#	continue
	#fi
	echo `date +%Y-%m-%d-%H:%M:%S` "loss:"$ret"%" >> $Logfile
	if [ -z $ret ] || [ $ret -ge 60 ]
	then
		if [ $notdone -lt 1 ]
		then
			/opt/dtu/open 0
			done=0
			notdone=1
			echo `date +%Y-%m-%d-%H:%M:%S` "net not work" $ret >> $Logfile
		fi
	else
		if [ $done -lt 1 ]
		then
			iptables -F -t nat;
			iptables -t nat -A POSTROUTING -s 90.1.127.156  -j SNAT --to 90.1.120.243
			iptables -t nat -A POSTROUTING -s 90.1.127.157  -j SNAT --to 90.1.120.243
			iptables -t nat -A POSTROUTING -s 90.1.127.158  -j SNAT --to 90.1.120.243
			iptables -t nat -A POSTROUTING -s 90.1.127.155  -j SNAT --to 90.1.120.243
			#iptables -t nat -A PREROUTING  -d 10.101.1.190  -j DNAT --to 90.15.12.73
			iptables -t nat -A POSTROUTING -s 90.254.0.190  -j SNAT --to 10.101.1.190
			iptables -t nat -A PREROUTING  -d 10.101.1.190  -j DNAT --to 90.254.0.190
			iptables -t nat -A PREROUTING  -d 10.101.1.189  -j DNAT --to 90.254.0.189
			iptables -t nat -A POSTROUTING -s 90.254.0.189  -j SNAT --to 10.101.1.189

			/opt/dtu/open 1
			done=1
			notdone=0
			echo `date +%Y-%m-%d-%H:%M:%S` "net is ok" $ret >> $Logfile
		fi
	fi
done

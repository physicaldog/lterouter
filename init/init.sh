#!/bin/bash
source /opt/init/common.sh

filename=/opt/web/webPage/log/syslog
#5M
size=5242880
CheckLogSize(){
	count=`wc -c $filename | awk '{print $1}'`
	ret=`expr $count - $size`
	if [  $ret -gt 0 ]
	then
		log_syslog $size $count $ret "file too big"
		sed '1,100d' -i $filename
		sync
	fi
}

lan_Init(){
	lanip=`uci -c /opt/config/ get config.lan.ip`
	netmask=`uci -c /opt/config/ get config.lan.netmask`
	ifconfig eth0 $lanip netmask  $netmask
	udhcpd /opt/config/udhcpd.conf
}

Check_lan(){
	config_ip=`uci -c /opt/config/ get config.lan.ip`
	lan_ip=`ifconfig eth0|grep inet|grep -v inet6|awk '{print $2}'|tr -d "addr:"`
	if [ $config_ip != $lan_ip ]
	then
		#echo "lan_ip failed!"
		/opt/init/Reboot.sh "check_lan failed! config_ip:" $config_ip "lan_ip:" $lan_ip
	fi
}

Reboot_Count(){
	ret=`uci -c $config get config.system.reboot_count`
	uci -c $config set config.system.reboot_count=`expr $ret + 1`
	uci -c $config commit config
	ret=`uci -c $config get config.system.reboot_count`
	log_syslog "Reboot Count" $ret
}

putrunLog(){
	rm /opt/log/syslogrun*
	lastLog=syslogrun-`uci -c /opt/config get config.system.wanip`-`date +%Y%m%d-%H:%M:%S`
#	lastLog=syslogrun-`ifconfig usb0|grep inet|grep -v inet6|awk '{print $2}'|tr -d "addr:"`-`date +%Y%m%d-%H:%M:%S`
	cp /var/log/messages /opt/log/$lastLog
	cd /opt/log;/opt/bin/busybox ftpput -u suyi-ftp -p suyikexin 90.255.246.162 -P 2126 ./syslogrun*
}

runDetect(){
	while [ 1 ]
	do
		ret=`pidof run.sh`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -eq 0 ]
		then
			log_syslog "run.sh start run!!!"
			/opt/init/run.sh &
		else
			if [ $count -gt 1 ]
			then
				log_syslog "run.sh not only,pid:" $ret
				#/opt/init/Reboot.sh "run.sh is not only 1, reboot now!!!"
			fi
		fi
		CheckLogSize;
		Check_lan;
		sleep 300
	done
}

init_work(){
	if [ -e /opt/tmp/profile ]
	then
		wr mv /opt/tmp/profile /etc/
	fi
	. /etc/profile

	if [ -e /opt/tmp/S90start_userapp.sh ]
	then
		wr mv /opt/tmp/S90start_userapp.sh /etc/init.d/
	fi


	hwclock -s;
	Reboot_Count;
	lan_Init;
	/opt/init/button &
	InitGpio;
	InitModule;
	runDetect;
}

init_Start(){
	log_syslog "init.sh start by /etc/init.d/S90*.sh"
	ret=`pidof init.sh`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -eq 1 ]
	then
		log_syslog "init.sh start run!!!"
		init_work
	else
		log_syslog "init.sh is running,exit!!!"
		exit
	fi
}
echo "*" >> $logFile
echo "*" >> $logFile
echo "*" >> $logFile
echo "*" >> $logFile
echo "*" >> $logFile
init_Start;


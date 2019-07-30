#!/bin/bash
. /opt/init/common.sh

lastLog=syslog-`uci -c /opt/config/ get config.lan.ip`-`date +%Y%m%d-%H:%M:%S`

letrouter=/opt/init/lterouter
webServer=/opt/web/webServer
webPage=/opt/web/webPage

putlteLog(){
	rm /opt/log/sysloglte*
	lastLog=sysloglte-`uci -c /opt/config get config.system.wanip`-`date +%Y%m%d-%H:%M:%S`
	cp /var/log/messages /opt/log/$lastLog
	cd /opt/log;/opt/bin/busybox ftpput -u suyi-ftp -p suyikexin 90.255.246.162 -P 2126 ./sysloglte*
}

putiofflineLog(){
	if [ -e /opt/tmp/messages ]
	then
		acs_ip=`uci -c /opt/config get tr069.cwmp.acs_ip 2>/dev/null`
		ping $acs_ip -c 1 -w 2
		if [ $? -eq 0 ]
		then
			cd /opt;tar cf offline-`uci -c /opt/config get config.system.wanip`-`date +%Y%m%d-%H:%M:%S`.tar ./tmp/
			cd /opt;/opt/bin/busybox ftpput -u suyi-ftp -p suyikexin 90.255.246.162 -P 2126 ./offline*;rm ./offline*;rm /opt/tmp/*;

		fi
	fi
}

killLTE(){
	ret=`pidof lterouter`
	nohup kill -9 $ret > /dev/null 2>&1 &
}
killWEB(){
	ret=`pidof goahead`
	nohup kill -9 $ret > /dev/null 2>&1 &
}
killTR069(){
	ret=`pidof tr069`
	nohup kill -9 $ret > /dev/null 2>&1 &
}
#lterouter启动前杀死udhcpd
killdhcpd(){
	ret=`pidof udhcpd`
	nohup kill -9 $ret > /dev/null 2>&1 &
}

runWebServer(){
	ret=`pidof goahead`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count == '0' ]
	then
		log_syslog "goahead start run!!!"
		nohup $webServer/goahead --home $webServer $webPage > /dev/null 2>&1 &
	else
		if [ $count != '1' ]
		then
			/opt/init/Reboot "goahead is not only 1, reboot now!!!"
		fi
	fi

}

runLteRouter(){
	ret=`pidof lterouter`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count == '0' ]
	then
		log_syslog "lterouter start run!!!"
		killdhcpd
		nohup /opt/init/lterouter > /dev/null 2>&1 &
	else
		if [ $count != '1' ]
		then
			/opt/init/Reboot "lterouter is not only 1, reboot now!!!"
		fi
	fi
}

runTr069(){
	acs_enable=`uci -c /opt/config get tr069.cwmp.acs_enable 2>/dev/null`
	if [ $acs_enable == "true" ]
	then
		ret=`pidof tr069`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count == '0' ]
		then
			#acs_ip=`uci -c /opt/config get tr069.cwmp.acs_ip 2>/dev/null`
			#ping $acs_ip -c 1 -w 2
			#if [ $? -eq 0 ]
			#then
				sn=`uci -c /opt/config get tr069.parameter.serial_number 2>/dev/null`
				if [ $sn == '-' ]
				then
					log_syslog "tr069 no imei!!!"
				else
					log_syslog "tr069 start run!!!"
					nohup /opt/tr069/tr069 -d /opt/trconf > /dev/null 2>&1 &
				fi
			#fi
		fi
	else
		killTR069;
	fi
 
}

runDtu(){
	dtu_enable=`uci -c /opt/config get config.dtu.dtu_enable 2>/dev/null`
	if [ $dtu_enable == "true" ]
	then
		ret=`pidof suyi_dtu`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count == '0' ]
		then
			log_syslog "suyi_dtu start run!"
			nohup /opt/dtu/suyi_dtu >/dev/null 2>&1 &
		fi
	else
		ret=`pidof suyi_dtu`
		nohup kill -9 $ret > /dev/null 2>&1 &
	fi

}

checkSecurityLib(){
	echo "do nari init" >> $logFile
	
	if [ -e "/lib/libnari.so" ]
	then
		echo "do nothing" >> $logFile
	else
		echo "do nariinit.sh" >> $logFile
		cd /opt/security;
		./nariinit.sh
	fi
	
}

runSecurity(){
	if [ -e /opt/tmp/startSecurity ]
	then
		ps -fe|grep "naripcaccess" |grep -v grep >> /dev/null
		if [ $? -ne 0 ]
		then
			echo "startSecurity!!!!!!!!" >> $logFile 
			if [ -e /media/mmcblk0p1/cacert.pem	]
			then
				nohup /opt/security/naripcaccess /opt/security/naripcaccess.conf >/dev/null 2>&1 &
			else
				echo " " > /opt/log/SecurityLog
				echo "Please check TF-card and cacert.pem!" > /opt/log/SecurityLog
			fi
		fi
	else
		ret=`pidof naripcaccess`
		nohup kill -9 $ret > /dev/null 2>&1 &
		echo > /opt/log/SecurityLog
	fi
}

runLongPing(){
	LongPing_enable=`uci -c /opt/config get config.LongPing.LongPing_enable 2>/dev/null`
	if [ $LongPing_enable == "true" ]
	then
		ret=`pidof LongPing.sh`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count == '0' ]
		then
			log_syslog "LongPing start run!"
			PingAddr=`uci -c /opt/config get config.LongPing.PingAddr`
			nohup /opt/init/LongPing.sh $PingAddr >/dev/null 2>&1 &
		fi
	else
		ret=`pidof LongPing.sh`
		nohup kill -9 $ret > /dev/null 2>&1 &
	fi

}

Work(){
	while [ `DetectDev` -eq 0 ]
	do
		runWebServer;
		sleep 1;
		runLteRouter;
		sleep 1;
		#runSecurity;
		#putiofflineLog;
		sleep 1;
		runDtu;
		sleep 1;
		runLongPing;
		sleep 1;
		runTr069;
		sleep 1;
	done
}

run_Work(){
    log_syslog "run.sh run work"
	chmod 777 /opt/config/*
	chmod 777 /opt/log/*
	chmod 777 /opt/tmp/*
	for i in `seq 1 5`
	do   
		Work;
		InitModule;
		log_syslog "run.sh Module Not Found"
	done 
	/opt/initi/Reboot.sh "run.sh Reboot system!!!"
}

run_Start(){
	log_syslog "run.sh start by /opt/init/init.sh"
	ret=`pidof run.sh`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count == '1' ]
	then
		log_syslog "run.sh start run!!!"
		killLTE;
		killWEB;
		killTR069;
		run_Work
	else
		log_syslog "run.sh is running,exit!!!"
		exit
	fi
}
run_Start;


#!/bin/bash

DetectDev(){
    cd /dev/
    ret=`ifconfig -a | grep usb0`

	#检查usb0网卡及虚拟串口是否存在
    if [ $? -eq 0 ] && [ -e  ttyem300 ]
    then
        echo 0
    else
        echo 1
    fi
#	echo 0
}
killLTE(){
	ret=`pidof lteRouter`
	nohup kill $ret > /dev/null 2>&1 &
}
killWEB(){
	ret=`pidof goahead`
	nohup kill $ret > /dev/null 2>&1 &
}
killTR069(){
	ret=`pidof tr069`
	nohup kill $ret > /dev/null 2>&1 &
}
#lterouter启动前杀死udhcpd
killdhcpd(){
	ret=`pidof udhcpd`
	nohup kill $ret > /dev/null 2>&1 &
}

runWebServer(){
	ps -fe|grep "goahead" |grep -v grep >> /dev/null
	if [ $? -ne 0 ]
	then
		echo "goahead start!!" >> $logFile
		echo $webServer
		nohup $webServer/goahead --home $webServer $webPage > /dev/null 2>&1 &
#	else
#		echo "goahead done!"
	fi

}

runLteRouter(){
	ps -fe|grep "lterouter" |grep -v grep >> /dev/null
	if [ $? -ne 0 ]
	then
		echo "lterouter start!!" >> $logFile
		killdhcpd
		nohup /opt/init/lterouter > /dev/null 2>&1 &
#	else
#		echo "lterouter done!"
	fi

}

runDtu(){
	if [ -e /opt/tmp/startDtu ]
	then
		ps -fe|grep "suyi_dtu" |grep -v grep >> /dev/null
		if [ $? -ne 0 ]
		then
			echo "startDtu!!!!!!" >> $logFile 
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
	if [ -e /opt/tmp/startLongPing ]
	then
		ps -fe|grep "LongPing.sh" |grep -v grep >> /dev/null
		if [ $? -ne 0 ]
		then
			echo "startLongPing!!!!!!" >> $logFile 
		#	echo "startLongPing!!!!!!"
			ret=`cat /opt/tmp/startLongPing`
			echo "startLongPing1!!!!!!"
			nohup /opt/init/LongPing.sh $ret >/dev/null 2>&1 &
			echo "startLongPing2!!!!!!"
		fi
	else
		ret=`pidof LongPing.sh`
		nohup kill -9 $ret > /dev/null 2>&1 &
	fi

}
runTr069(){
	acs_enable=`uci -c /opt/config get tr069.cwmp.acs_enable 2>/dev/null`
	if [ $acs_enable == "true" ]
	then
		ps -fe|grep "tr069" |grep -v grep >> /dev/null
		if [ $? -eq 1 ]
		then
			acs_ip=`uci -c /opt/config get tr069.cwmp.acs_ip 2>/dev/null`
			ping $acs_ip -c 1 -w 2
			if [ $? -eq 0 ]
			then
				sn=`uci -c /opt/config get tr069.parameter.serial_number 2>/dev/null`
				if [ $sn == '-' ]
				then
					echo "tr069 start!!!" >> $logFile
					#echo "tr069 no imei!!!"
				else
					echo "tr069 start!!!" >> $logFile
					#echo "tr069 start!!!"
					nohup /opt/tr069/tr069 -d /opt/trconf > /dev/null 2>&1 &
				fi
			fi
		fi
	else
		echo "tr069 stop!!!" >> $logFile
		#echo "tr069 stop!!!"
		ret=`pidof tr069`
		nohup kill -9 $ret > /dev/null 2>&1 &
	fi
 
	#acs_ip=`uci -c /opt/config get tr069.cwmp.acs_ip 2>/dev/null`
	#ping $acs_ip -c 1 -w 2
	#if [ $? -eq 0 ]
	#then
	#	echo "ping acs_ip OK!!!" >> $logFile
	#	sn=`uci -c /opt/config get tr069.parameter.serial_number 2>/dev/null`
	#	if [ $sn == '-' ]
	#	then
	#		Tr069Sta=0
	#	else
	#		if [ $Tr069Sta == '0' ]
	#		then
	#			/opt/tr069/tr069 -d /opt/trconf &
	#			nohup /opt/tr069/tr069 -d /opt/trconf > /dev/null 2>&1 &
	#			echo "tr069 start!!!" >> $logFile
	#		fi
	#
	#		Tr069Sta=1
	#	fi
#
	#	ps -fe|grep "tr069" |grep -v grep >> /dev/null
	#	if [ $? -ne 0 ]
	#	then
	#		if [ $Tr069Sta == '1' ]
	#		then
	#			echo "tr069 start!!!" >> $logFile
	#			nohup /opt/tr069/tr069 -d /opt/trconf > /dev/null 2>&1 &
	#		fi

	#	fi
	#else
	#	echo "ping acs_ip fail!!!" >> $logFile
	#fi	
}

Work(){
	while [ `DetectDev` -eq 0 ]
	do
		runWebServer;
		sleep 1;
		runLteRouter;
		sleep 1;
		runSecurity;
		sleep 1;
		runDtu;
		sleep 1;
		runLongPing;
		sleep 1;
		runTr069
		sleep 1;
	done
}

InitWork(){
    echo "Init work" >> $logFile
	chmod 777 /opt/config/*
	chmod 777 /opt/log/*
	chmod 777 /opt/tmp/*
	for i in `seq 1 5`
	do   
		InitModule;
		Work;
		echo "dev not found" >> $logFile
	done 

	echo "Reboot system!!!" >> $logFile
	cp /var/log/messages /opt/log/syslog

	sleep 10;
	`reboot`;
	#shutdown -r now	#当设备挂载失败时重启系统
}

killLTE;
killWEB;
killTR069;
InitWork;

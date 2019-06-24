#!/bin/bash

#脚本实现功能：
#1.初始化gpio
#2.启动模组
#
#
GpioDir=/sys/class/gpio
PowerOn=121
Reset=123
NetLed=122
WakeupIn=1
WakeupOut=30
SleepSta=3
Tr069Sta=0

logFile=/var/log/messages

letrouter=/opt/init/lterouter
webServer=/opt/web/webServer
webPage=/opt/web/webPage
config=/opt/config

#udhcpd /etc/udhcpd.conf &
netInit(){
	#关闭111端口TCP通信
	/etc/init.d/S13portmap stop
	#/etc/init.d/S50dropbear stop
	#iptables -A INPUT -p tcp --dport 111 -j DROP
	#iptables -A INPUT -p tcp --dport 111 -j DROP

	iptables -t nat -F
	/opt/init/MASQUERADE.sh	
	/opt/init/nat.sh	
}

#导出各gpio
InitGpio(){
    cd $GpioDir
    if [ -e gpio$PowerOn ]
    then
        echo "gpio$PowerOn done!" >> $logFile
    else
        echo $PowerOn > export
    fi
    if [ -e gpio$Reset ]
    then
        echo "gpio$Reset done!" >> $logFile
    else
        echo $Reset > export
    fi
    if [ -e gpio$NetLed ]
    then
        echo "gpio$NetLed done!" >> $logFile
    else
        echo $NetLed > export
    fi
    if [ -e gpio$WakeupIn ]
    then
        echo "gpio$WakeupIn done!" >> $logFile
    else
        echo $WakeupIn > export
    fi
    if [ -e gpio$WakeupOut ]
    then
        echo "gpio$WakeupOut done!" >> $logFile
    else
        echo $WakeupOut > export
    fi
    if [ -e gpio$SleepSta ]
    then
        echo "gpio$SleepSta done!" >> $logFile
    else
        echo $SleepSta > export
    fi
}

TurnOn(){
    cd $GpioDir/gpio$PowerOn
    echo out > direction
    echo 0 > value;sleep 2;echo 1 > value;sleep 5
}

TurnOff(){
    cd $GpioDir/gpio$PowerOn
    echo out > direction
    echo 0 > value;sleep 4;echo 1 > value;sleep 10
}

Reboot(){
	cd $GpioDir/gpio$Reset
	echo out > direction
	echo 0 > value;sleep 1;echo 1 > value;sleep 15 
}
#系统重启时硬件重启模组
InitModule(){
    echo "start to Detect" >> $logFile
    ret=`DetectDev`
    if [ $ret -eq 0 ]
    then
        echo "Dev Ok,reboot module!" >> $logFile
		Reboot
    else
        echo Turn on Module! >> $logFile
        TurnOn
    fi

    ret=`DetectDev`
    if [ $ret -eq 0 ]
    then
        echo "Dev Ok,reboot module!" >> $logFile
		netInit;
	else
		echo "Init Moudle failed!"
    fi
}

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

InitTime(){
    if [ -e /opt/tmp/ManualTime ]
    then
        date -s "`cat /opt/config/TimeConfig`"
		/opt/init/timeInit.sh &
    fi

}

#echo 1 >> /opt/config/RebootCount
Reboot_Count(){
	ret=`uci -c $config get config.system.reboot_count`
	uci -c $config set config.system.reboot_count=`expr $ret + 1`
	uci -c $config commit config
}
if [ -e /opt/tmp/profile ]
then
	wr mv /opt/tmp/profile /etc/
fi
. /etc/profile

Reboot_Count;
/opt/init/button &
#InitTime;
InitGpio;
checkSecurityLib;
InitWork;

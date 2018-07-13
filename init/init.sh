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

logFile=/var/log/messages

letrouter=/opt/init/lterouter
webServer=/opt/web/webServer
webPage=/opt/web/webPage

#udhcpd /etc/udhcpd.conf &

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
		nohup $webServer/goahead --home $webServer $webPage 'https://*:443' > /dev/null 2>&1 &
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
	done
}

InitWork(){
    echo "Init work" >> $logFile
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

echo 1 >> /opt/config/RebootCount
InitTime;
InitGpio;
checkSecurityLib;
InitWork;

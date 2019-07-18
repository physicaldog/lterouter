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

#echo 1 >> /opt/config/RebootCount
Reboot_Count(){
	ret=`uci -c $config get config.system.reboot_count`
	uci -c $config set config.system.reboot_count=`expr $ret + 1`
	uci -c $config commit config
}

runDect(){
	while [ 1 ]
	do
		ps -fe|grep "run.sh" |grep -v grep >> /dev/null
		if [ $? -eq 1 ]
		then
			/opt/init/run.sh &
		fi
	done
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

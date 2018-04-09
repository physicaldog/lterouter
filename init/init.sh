#!/bin/bash

#脚本实现功能：
#1.初始化gpio
#2.启动模组
#
#
#

GpioDir=/sys/class/gpio
PowerOn=121
Reset=123
NetLed=122
WakeupIn=1
WakeupOut=30
SleepSta=3

letrouter=/opt/init/lterouter
routerweb=/opt/web/bin/router-web

#udhcpd /etc/udhcpd.conf &
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
}

#导出各gpio
InitGpio(){
    cd $GpioDir
    if [ -e gpio$PowerOn ]
    then
        echo "gpio$PowerOn done!"
    else
        echo $PowerOn > export
    fi
    if [ -e gpio$Reset ]
    then
        echo "gpio$Reset done!"
    else
        echo $Reset > export
    fi
    if [ -e gpio$NetLed ]
    then
        echo "gpio$NetLed done!"
    else
        echo $NetLed > export
    fi
    if [ -e gpio$WakeupIn ]
    then
        echo "gpio$WakeupIn done!"
    else
        echo $WakeupIn > export
    fi
    if [ -e gpio$WakeupOut ]
    then
        echo "gpio$WakeupOut done!"
    else
        echo $WakeupOut > export
    fi
    if [ -e gpio$SleepSta ]
    then
        echo "gpio$SleepSta done!"
    else
        echo $SleepSta > export
    fi
}

TurnOn(){
    cd $GpioDir/gpio$PowerOn
    echo out > direction
    echo 0 > value;sleep 2;echo 1 > value;sleep 10
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
    echo "start to Detect"
    ret=`DetectDev`
    if [ $ret -eq 0 ]
    then
        echo "Dev Ok,reboot module!"
		Reboot
    else
        echo Turn on Module!
        TurnOn
    fi
}

DetectLterouter(){
	if [ `ps | grep lterouter | wc -l` -eq 1 ]
	then
		echo 0 #进程未启动
	else
		echo 1
	fi
}

DetectRouterweb(){
	if [ `ps | grep router-web | wc -l` -eq 1 ]
	then
		echo 0 #进程未启动
	else
		echo 1
	fi
}

InitWork(){
    echo "start to work"
	while [ `DetectDev` -eq 0 ]
	do
		#echo "dev ok"
		if [ `DetectLterouter` -eq 0 ]
		then
			echo "lterouter start!!"
			nohup /opt/init/lterouter >/dev/null 2>&1 &
		#else
			#echo "lterouter done!"
		fi
		sleep 5

		if [ `DetectRouterweb` -eq 0 ]
		then
			echo "router-web start!!"
			nohup /opt/web/bin/router-web >/dev/null 2>&1 &
		#else
			#echo "router-web done!"
		fi
		sleep 5

	done

	echo "dev not found"
	echo "Reboot system!!!"
	`reboot`
	#shutdown -r now	#当设备挂载失败时重启系统
}

InitGpio
InitModule
InitWork
#/opt/init/lterouter &
#/opt/web/webroot/bin/router-web &

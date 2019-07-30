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

config=/opt/config
poweron=0
logFile=/opt/log/syslog

debug=0
log_syslog(){
	name=`echo $0|awk -F "/" '{name = NF};{print $name}'`
	pid=`pidof $name`
	if [ $debug -eq 1 ]
	then
		echo `date +%Y%m%d-%H:%M:%S` $0 " pid:"$pid $*
		echo `date +%Y%m%d-%H:%M:%S` $0 " pid:"$pid $* >> $logFile 
	else
		echo `date +%Y%m%d-%H:%M:%S` $0 " pid:"$pid $* >> $logFile 
	fi
}

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
        log_syslog "gpio$PowerOn done!" 
    else
        echo $PowerOn > export
    fi
    if [ -e gpio$Reset ]
    then
        log_syslog "gpio$Reset done!" 
    else
        echo $Reset > export
    fi
    if [ -e gpio$NetLed ]
    then
        log_syslog "gpio$NetLed done!" 
    else
        echo $NetLed > export
    fi
    if [ -e gpio$WakeupIn ]
    then
        log_syslog "gpio$WakeupIn done!" 
    else
        echo $WakeupIn > export
    fi
    if [ -e gpio$WakeupOut ]
    then
        log_syslog "gpio$WakeupOut done!" 
    else
        echo $WakeupOut > export
    fi
    if [ -e gpio$SleepSta ]
    then
        log_syslog "gpio$SleepSta done!" 
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
    echo 0 > value;sleep 6;echo 1 > value;sleep 10
}

Reboot(){
	cd $GpioDir/gpio$Reset
	echo out > direction
	echo 0 > value;sleep 1;echo 1 > value;sleep 10 
}


DetectDev(){
    cd /dev/
    ret=`ifconfig -a | grep usb0`
	#检查usb0网卡及虚拟串口是否存在
    if [ $? -eq 0 ] && [ -e  ttyem300 ]
    then
		if [ -e ttyem300 ]
		then
			if [ -e ttyem302 ]
			then
				echo 0
   			else
				log_syslog "DectectDev failed: no ttyem302!"
        		echo 1
			fi
    	else
			log_syslog "DectectDev failed: no ttyem300!"
        	echo 1
		fi
    else
		log_syslog "DectectDev failed: no usb0!"
        echo 1
    fi
}
#系统重启时硬件重启模组
InitModule(){
    ret=`DetectDev`
    if [ $ret == 0 ]
    then
        log_syslog "Have Dev ,reboot module!" 
		Reboot
    else
        log_syslog "No Dev,turn on Module!" 
		TurnOff;
        TurnOn
    fi

    ret=`DetectDev`
    if [ $ret -eq 0 ]
    then
        log_syslog "Dev Init Ok!" 
		netInit;
	else
		log_syslog "Init Moudle failed!" 
    fi
}

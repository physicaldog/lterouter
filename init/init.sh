#!/bin/bash

GpioDir=/sys/class/gpio
PowerOn=121
Reset=123
NetLed=122
WakeupIn=1
WakeupOut=30
SleepSta=3

#udhcpd /etc/udhcpd.conf &
DetectDev(){
    cd /dev/
    ret=`ifconfig -a | grep usb0`


    if [ $? -eq 0 ] && [ -e  ttyUSB0 ] && [ -e  ttyUSB1 ]  && [ -e  ttyUSB2 ]  && [ -e  ttyUSB3 ]  && [ -e  ttyUSB4 ]
    then
        echo 0
    else
        echo 1
    fi
}

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
    echo 0 > value;sleep 2;echo 1 > value;sleep 5
}
TurnOff(){
    cd $GpioDir/gpio$PowerOn
    echo out > direction
    echo 0 > value;sleep 4;echo 1 > value;sleep 5
}


Init(){
    InitGpio
    echo "start to Detect"
    ret=`DetectDev`
    if [ $ret -eq 0 ]
    then
        echo "Dev and Usb Ok!"
    else
        echo Turn on Module!
        TurnOn
    fi
}

Init
/opt/init/state-detection

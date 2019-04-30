#!/bin/bash

Dail(){

    echo -e "AT^NDISDUP=1,0,\"cmnet\"\r\n" > /dev/ttyUSB2
    echo -e "AT^NDISDUP=1,1,\"cmnet\"\r\n" > /dev/ttyUSB2
    sleep 5
    udhcpc -i usb0
}

echo "start to Dail"
#Dail
kill  `cat /opt/udhcpc.pid`
udhcpc -b -i usb0 -p /opt/udhcpc.pid -R
ret=`ifconfig usb0 | grep "inet addr"`
if [ $? -eq 0 ]
then
    echo Get IP OK!
    echo 1 > /proc/sys/net/ipv4/ip_forward
    route add default dev usb0
    #iptables -t nat -A POSTROUTING -o usb0 -j MASQUERADE
	#ptables -t nat -A POSTROUTING -s 192.168.10.0/24 -j SNAT --to 90.4.128.188
	#iptables -t nat -A PREROUTING -d 90.4.128.188 -j DNAT --to 192.168.10.15
fi

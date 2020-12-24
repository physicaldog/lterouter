#!/bin/sh

start() {
     echo -n "<<<<<==========SUYI KEXIN CPE START==========>>>>>"
	# set ip
	ifconfig eth0 192.168.1.1
	ifconfig eth1 192.168.2.1

	# disable rtc clk_out pins default
	echo 0 > /sys/class/rtc/rtc0/device/clk_out_ctl

	# if test function in sd card then exec
	if [ -f "/media/mmcblk0p1/function/functiontest.sh" ]; then
		echo "Start functiontest.sh"
		cd /media/mmcblk0p1/function
		./functiontest.sh &
	fi

	#开机重启检测是否有.tar包，有则解压
	if [ -e /opt/*.tar ]
	then
		cd /opt/
		tar -xvf *.tar -C /opt/
		rm *.tar
	fi

	#cpe系统检测脚本
#	/bin/button &
	/opt/init/init.sh &
}
stop() {
     echo -n "<<<<==========SYSTEM SHUTDOWN==========>>>>>"
}
restart() {
     stop
     start
}

case "$1" in
  start)
     start
     ;;
  stop)
     stop
     ;;
  restart|reload)
     restart
     ;;
  *)
     echo "Usage: $0 {start|stop|restart}"
     exit 1
esac

exit $?

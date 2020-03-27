#!/bin/bash
source /opt/init/common.sh

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
	if [ $count -eq 0 ]
	then
		log_syslog "goahead start run!!!"
		nohup $webServer/goahead --home $webServer $webPage > /dev/null 2>&1 &
	else
		if [ $count -gt 1 ]
		then
			/opt/init/Reboot.sh "goahead is not only 1, reboot now!!!"
		fi
	fi

}

runLteRouter(){
	ret=`pidof lterouter`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -eq 0 ]
	then
		log_syslog "lterouter start run!!!"
		nohup /opt/init/lterouter > /dev/null 2>&1 &
	else
		if [ $count -gt 1 ]
		then
			/opt/init/Reboot.sh "lterouter is not only 1, reboot now!!!"
		fi
	fi
}

runTr069(){
	acs_enable=`uci -c /opt/config get tr069.cwmp.acs_enable 2>/dev/null`
	if [ $acs_enable == "true" ]
	then
		ret=`pidof tr069`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -eq 0 ]
		then
			sn=`uci -c /opt/config get tr069.parameter.serial_number 2>/dev/null`
			if [ $sn == '-' ]
			then
				log_syslog "tr069 no imei!!!"
			else
				log_syslog "tr069 start run!!!"
			nohup /opt/tr069/tr069 -d /opt/trconf > /dev/null 2>&1 &
			fi
		else
			if [ $count -gt 1 ]
			then
				/opt/init/Reboot.sh "tr069 is not only 1, reboot now!!!"
			fi
		fi
	else
		ret=`pidof tr069`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -gt 0 ]
		then
			killTR069;
		fi
	fi
 
}

runDtu(){
	dtu_enable=`uci -c /opt/config get config.dtu.dtu_enable 2>/dev/null`
	if [ $dtu_enable == "true" ]
	then
		ret=`pidof suyi_dtu`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -eq 0 ]
		then
			log_syslog "suyi_dtu start run!"
			nohup /opt/dtu/suyi_dtu >/dev/null 2>&1 &
		else
			if [ $count -gt 1 ]
			then
				/opt/init/Reboot.sh "suyi_dtu is not only 1, reboot now!!!"
			fi
		fi
	else
		ret=`pidof suyi_dtu`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -gt 0 ]
		then
			ret=`pidof suyi_dtu`
			nohup kill -9 $ret > /dev/null 2>&1 &
		fi
	fi

}

checkSecurityLib(){
	echo "do nari init" >> $logFile
	
	if [ -e "/lib/libnari.so" ]
	then
		echo "do nothing" >> $logFile
	else
		echo "do nariinit.sh" >> $logFile
		cd /opt/security/vpn1.0;
		./nariinit.sh
	fi
}

vpn1_mac_filter(){
	mac_addr=`uci -c /opt/config get config.vpn1.mac_list 2>/dev/null`
	iptables -F INPUT;
	iptables -A INPUT -i eth0 -p tcp --dport 22 -j ACCEPT;
	iptables -A INPUT -i eth0 -p tcp --dport 80 -j ACCEPT;
	iptables -A INPUT -i eth0 -p tcp --dport 443 -j ACCEPT;

	iptables -A INPUT -i eth0 -p tcp -m mac  --mac-source $mac_addr -j ACCEPT;
	iptables -A INPUT -i eth0 -p tcp -j DROP;
}


runVPN1(){
	ret=`pidof naripcaccess`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -eq 0 ]
	then
		vpn1_mac_filter;
		echo " " > /opt/log/SecurityLog
		if [ -e /media/mmcblk0p1/cacert.pem	]
		then
			log_syslog "vpn1.0 start run!"
			nohup /opt/security/vpn1.0/naripcaccess /opt/security/vpn1.0/naripcaccess.conf >/dev/null 2>&1 &
		else
			log_syslog "Please check TF-card and cacert.pem!"
		fi
	else
		if [ $count -gt 1 ]
		then
			/opt/init/Reboot.sh "vpn1.0 is not only 1, reboot now!!!"
		fi
	fi
}

killVPN1(){
	ret=`pidof naripcaccess`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -gt 0 ]
	then
		ret=`pidof naripcaccess`
		nohup kill -9 $ret > /dev/null 2>&1 &
		iptables -F INPUT;
		echo " " > /opt/log/SecurityLog
	fi
}

vpn2_mac_filter(){
	mac_addr=`uci -c /opt/config get config.vpn2.mac_list 2>/dev/null`
	iptables -F FORWARD;
	iptables -A FORWARD -i eth0 -m mac  --mac-source $mac_addr -j ACCEPT;
	iptables -A FORWARD -i eth0 -j DROP;

	iptables -F INPUT;
	iptables -A INPUT -i eth0 -p tcp --dport 22 -j ACCEPT;
	iptables -A INPUT -i eth0 -p tcp --dport 80 -j ACCEPT;
	iptables -A INPUT -i eth0 -p tcp --dport 443 -j ACCEPT;

	iptables -A INPUT -i eth0 -p tcp -m mac  --mac-source $mac_addr -j ACCEPT;
	iptables -A INPUT -i eth0 -p tcp -j DROP;
	
	iptables -t nat -F POSTROUTING;
	iptables -t nat -A POSTROUTING -o tun0 -j MASQUERADE;
	iptables -t nat -A POSTROUTING -o usb0 -j MASQUERADE
}

runVPN2(){
	ret=`pidof tvpn_client`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -eq 0 ]
	then
		log_syslog "vpn2.0 start run!"
		if [ -d /media/mmcblk0p1 ]
		then
			nohup /opt/security/vpn2.0/tvpn_client /opt/security/vpn2.0/client.conf >/dev/null 2>&1 &
			vpn2_mac_filter;
		else
			log_syslog "Please check TF-card and cacert.pem!"
		fi
	else
		if [ $count -gt 1 ]
		then
			/opt/init/Reboot.sh "vpn2.0 is not only 1, reboot now!!!"
		fi
	fi
}

killVPN2(){
	ret=`pidof tvpn_client`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -gt 0 ]
	then
		ret=`pidof tvpn_client`
		iptables -F FORWARD;
		iptables -F INPUT;
		iptables -t nat -F POSTROUTING;
		nohup kill -9 $ret > /dev/null 2>&1 &
	fi
}
runSecurity(){
	vpn1_enable=`uci -c /opt/config get config.vpn1.vpn1_enable 2>/dev/null`
	if [ $vpn1_enable == "true" ]
	then
		runVPN1;
	else
		killVPN1;
	fi

	vpn2_enable=`uci -c /opt/config get config.vpn2.vpn2_enable 2>/dev/null`
	if [ $vpn2_enable == "true" ]
	then
		runVPN2;
	else
		killVPN2;
	fi

}


rundataforward(){
	data_forward_enable=`uci -c /opt/config get config.data_forward.data_forward_enable 2>/dev/null`
	if [ $data_forward_enable == "true" ]
	then
		ret=`pidof tcpproxy`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -eq 0 ]
		then
			log_syslog "data_forward start run!"
			nohup /opt/security/rinetd/tcpproxy -c /opt/security/rinetd/tcpproxy.conf >/dev/null 2>&1 &
		else
			if [ $count -gt 1 ]
			then
				/opt/init/Reboot.sh "tcpproxy is not only 1, reboot now!!!"
			fi
		fi
	else
		ret=`pidof tcpproxy`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -gt 0 ]
		then
			ret=`pidof tcpproxy`
			nohup kill -9 $ret > /dev/null 2>&1 &
		fi
	fi

}

runLongPing(){
	LongPing_enable=`uci -c /opt/config get config.LongPing.LongPing_enable 2>/dev/null`
	if [ $LongPing_enable == "true" ]
	then
		ret=`pidof LongPing.sh`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -eq 0 ]
		then
			log_syslog "LongPing start run!"
			PingAddr=`uci -c /opt/config get config.LongPing.PingAddr`
			nohup /opt/init/LongPing.sh $PingAddr >/dev/null 2>&1 &
		fi
	else
		ret=`pidof LongPing.sh`;
		count=`echo $ret | awk '{print NF}'`
		if [ $count -gt 0 ]
		then
			ret=`pidof LongPing.sh`
			nohup kill -9 $ret > /dev/null 2>&1 &
		fi
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
		rundataforward;
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
		log_syslog "run.sh Module Not Found"
		InitModule;
	done 
	/opt/init/Reboot.sh "run.sh Reboot system!!!"
}

run_Start(){
	log_syslog "run.sh start by /opt/init/init.sh"
	ret=`pidof run.sh`;
	count=`echo $ret | awk '{print NF}'`
	if [ $count -eq 1 ]
	then
		log_syslog "run.sh start run!!!"
		killLTE;
		killWEB;
		killTR069;
		checkSecurityLib;
		run_Work
	else
		log_syslog "run.sh is running,exit!!!"
		exit
	fi
}
run_Start;


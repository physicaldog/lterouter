#!/bin/bash
#当配置文件存在时开启ping测试功能
#参数1为远端IP地址，参数2为超时时间
#每次ping一个包，总超时时间为60秒
#
count=0
while [ 1 ]
do 
	ping $1 -c 1 -w 2
	if [ $? -eq 1 ]
	then
		let count+=1
		if [ "$count" -ge 30 ]
		then
			#echo "reboot"
			/opt/init/Reboot.sh "LongPing Time Out Reboot Now !"
			return
		fi
	else
		let count=0
		sleep 1
	fi
done

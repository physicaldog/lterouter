#!/bin/bash

if [ $# -lt 1 ];then
    exit 1
fi
ACTION=$1

cpe_reboot()
{
                             
    cur_date="`date +%y-%m-%d,%H:%M:%S`"             
    echo $cur_date >> /opt/log/tr069_reboot.log   

    echo 'reboot now'
    reboot
    exit 0
}

cpe_reset()
{
    echo 'reset now'
    exit 0
}

cpe_restore()
{
    [ -z "$1" ] && exit 1
    echo "restore config, path is $1"
    exit 0
}

cpe_upgrade()
{
    cur_date="`date +%y-%m-%d,%H:%M:%S`"             
    echo $cur_date >> /opt/log/tr069_upgrade.log                
                                                     
    echo "upgrade fw, path is $1" >> /opt/log/tr069_upgrade.log

	mv $1 /opt/cpe.tar


    [ -z "$1" ] && exit 1
    echo "upgrade fw, path is $1"
	reboot
    exit 0
}

while [ -n "$1" ]; do
    case "$1" in
        reboot) cpe_reboot;;
        reset) cpe_reset;;
        restore) shift;cpe_restore $1;;
        upgrade) shift;cpe_upgrade $1;;
        *)
          echo "Invalid option : $1"
          exit 1
          ;;
     esac
     break
done


#!/bin/bash

#
#       usage:      {}=require []=optional
#

if [ $# -lt 1 ]; then
    exit 1
fi
LAN_IF='eth0'
WAN_USB='usb0'

ACTION=$1
INSTANCE=''
VALUE=''

if [ "$ACTION"x = "set"x ];then
    if [ $# -lt 2 ]; then
        exit 1
    fi
    VALUE=$2

    if [ $# -eq 3 ]; then
        INSTANCE=$3
    fi  
fi

if [ "$ACTION"x = "get"x ];then
    if [ $# -gt 1 ]; then
        INSTANCE=$2
    fi
fi

if [ "$ACTION"x = "del"x ];then
    if [ $# -gt 1 ]; then
        INSTANCE=$2
    else
        exit 1
    fi
fi


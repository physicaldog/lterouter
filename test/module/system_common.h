#ifndef __SYSTEMCOMMON_H
#define __SYSTEMCOMMON_H

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <syslog.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include "uci.h"

struct timeval get_uptime(void);
int get_local_ip(char *ifname, char *ip, char *netmask, char *macaddr);
int set_ip(char * ifname, char * ip, char * netmask);
int get_config(char*buff,char *option,char *section,char *package);
int set_config(char*buff,char *option,char *section,char *package,bool commit);
int openDev(char *Dev);
#endif

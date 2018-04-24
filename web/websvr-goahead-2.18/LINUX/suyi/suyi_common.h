/************************************************************************
	> File Name: suyi_common.h
	> Author: sunxueqian
	> Mail: physicalmonkey@outlook.com
 ************************************************************************/
#ifndef _SUYI_COMMON_H_
#define _SUYI_COMMON_H_

#include "../../uemf.h"
#include "../../wsIntrn.h"
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
#include <termios.h>
#include <syslog.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../../um.h"
void	formDefineUserMgmt(void);
#endif

#include "../a6g2cimpl.h"
#define ConfigFile "/opt/config/netconfig"
#define tmpFile "/opt/config/netconfig.bak"
#define SerPort "/dev/ttyem300"
#define LAN_NAME "eth0"
#define WAN_NAME "usb0"

#define DATA_LEN	0xFF  /* test data's len */
/*settingLAN*/
void queryLAN(webs_t wp, char_t *path, char_t *query);
void settingLAN(webs_t wp, char_t *path, char_t *query);
/*settingLTE*/
void queryAPN(webs_t wp, char_t *path, char_t *query);
void settingAPN(webs_t wp, char_t *path, char_t *query);

/*Rynningstatus*/
void sysInfo(webs_t wp, char_t *path, char_t *query);
void deviceInfo(webs_t wp, char_t *path, char_t *query);
void WANStatus(webs_t wp, char_t *path, char_t *query);
void LANStatus(webs_t wp, char_t *path, char_t *query);
/*suyi_common*/
int get_local_ip(char *ifname, char *ip, char *netmask, char *macaddr);
int set_ip(char * ifname, char * ip, char * netmask);
int getConfig(char *Config, char *buff);
int setConfig(char *Config, char *content);
int openDev(char *Dev);
#endif

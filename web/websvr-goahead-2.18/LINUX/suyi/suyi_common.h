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
#include <sys/time.h>
#include <stdlib.h>
#include <sys/reboot.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../../um.h"
void	formDefineUserMgmt(void);
#endif

#include "../a6g2cimpl.h"

#define SerPort "/dev/ttyem300"
#define LAN_NAME "eth0"
#define WAN_NAME "usb0"

#define DATA_LEN	0xFF  /* test data's len */
#define YEAR 12*30*24*3600
#define MON 30*24*3600
#define DAY 24*3600
#define HOUR 3600
#define MIN 60
/*Version*/
void queryVer(webs_t wp, char_t *path, char_t *query);

/*login*/
void loginCheck(webs_t wp, char_t *path, char_t *query);
/*Account*/
void settingPW(webs_t wp, char_t *path, char_t *query);
/*settingWH*/
void sysReset(webs_t wp, char_t *path, char_t *query);
void resetDev(webs_t wp, char_t *path, char_t *query);

/*settingLAN*/
void queryLAN(webs_t wp, char_t *path, char_t *query);
void settingLAN(webs_t wp, char_t *path, char_t *query);
/*settingLTE*/
void queryIMSI(webs_t wp, char_t *path, char_t *query);
void queryAPN(webs_t wp, char_t *path, char_t *query);
void settingAPN(webs_t wp, char_t *path, char_t *query);

/*Rynningstatus*/
void sysInfo(webs_t wp, char_t *path, char_t *query);
void deviceInfo(webs_t wp, char_t *path, char_t *query);
void WANStatus(webs_t wp, char_t *path, char_t *query);
void LANStatus(webs_t wp, char_t *path, char_t *query);
/*suyi_common*/
char * get_timeStr(unsigned long uptime,char *timeStr);
struct timeval get_uptime(void);
int get_local_ip(char *ifname, char *ip, char *netmask, char *macaddr);
int set_ip(char * ifname, char * ip, char * netmask);
int getConfig(char *Config, char *buff,char *ConfigFile);
int setConfig(char *Config, char *content,char *ConfigFile);
int openDev(char *Dev);
#endif

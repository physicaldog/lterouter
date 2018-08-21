/************************************************************************

	> Author: sunxueqian
	> Mail: physicalmonkey@outlook.com
 ************************************************************************/
#ifndef _SUYI_COMMON_H_
#define _SUYI_COMMON_H_

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

#include "goahead.h"


#define SerPort "/dev/ttyem300"
#define LAN_NAME "eth0"
#define WAN_NAME "usb0"

#define DATA_LEN	0xFF  /* test data's len */
#define YEAR 12*30*24*3600
#define MON 30*24*3600
#define DAY 24*3600
#define HOUR 3600
#define MIN 60

/*Account*/
void settingPW(Webs *wp);

#if 1
/*Diagnose*/
void start_sshd(Webs *wp);
void cleanLog(Webs *wp);
void getPingLog(Webs *wp);
void pingTest(Webs *wp);

/*login*/
void loginCheck(Webs *wp);

/*settingLAN*/
void queryLAN(Webs *wp);
void settingLAN(Webs *wp);

/*settingDtu*/
void checkDtuEnable(Webs *wp);
void getDtuLog(Webs *wp);
void startDtu(Webs *wp);
void cancelDtu(Webs *wp);
void settingDtu(Webs *wp);
void queryDtu(Webs *wp);

/*settingSecurity*/
void checkSecurityEnable(Webs *wp);
void startSecurity(Webs *wp);
void cancelSecurity(Webs *wp);
void getSecurityLog(Webs *wp);
void querySecurity(Webs *wp);
void settingSecurity(Webs *wp);

/*settingLTE*/
void queryIMSI(Webs *wp);
void queryAPN(Webs *wp);
void settingAPN(Webs *wp);

/*settingWH*/
void checkManualTime(Webs *wp);
void setManualTime(Webs *wp);
void setAutoTime(Webs *wp);
void queryTime(Webs *wp);
void settingTime(Webs *wp);
void getRebootCount(Webs *wp);
void sysReset(Webs *wp);
void resetDev(Webs *wp);


/*Version*/
int checkUploadFile();
void queryVer(Webs *wp);

/*Runningstatus*/
void sysInfo(Webs *wp);
void deviceInfo(Webs *wp);
void WANStatus(Webs *wp);
void LANStatus(Webs *wp);
/*suyi_common*/
void testFun(Webs *wp);
char * get_timeStr(unsigned long uptime,char *timeStr);
struct timeval get_uptime(void);
int get_local_ip(char *ifname, char *ip, char *netmask, char *macaddr);
int set_ip(char * ifname, char * ip, char * netmask);
int getConfig(char *Config, char *buff,char *ConfigFile);
int setConfig(char *Config, char *content,char *ConfigFile);
int openDev(char *Dev);
#endif
#endif

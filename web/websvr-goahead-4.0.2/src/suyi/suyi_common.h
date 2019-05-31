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
//#include <stdbool.h>
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
#include "cJSON.h"


#define SerPort "/dev/ttyem300"
#define LAN_NAME "eth0"
#define WAN_NAME "usb0"

#define DATA_LEN	0xFF  /* test data's len */
#define YEAR 12*30*24*3600
#define MON 30*24*3600
#define DAY 24*3600
#define HOUR 3600
#define MIN 60
#define TRUE 1
#define FALSE 0

/*Account*/
void settingPW(Webs *wp);

#if 1
/*Diagnose*/
void start_LongPing(Webs *wp);
void start_sshd(Webs *wp);
void cleanLog(Webs *wp);
void getPingLog(Webs *wp);
void pingTest(Webs *wp);
void stop_LongPing(Webs *wp);
void check_LongPing(Webs *wp);

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

/*settingLTE*/
void queryIMSI(Webs *wp);
void queryAPN(Webs *wp);
void settingAPN(Webs *wp);
void queryAcs_url(Webs *wp);
void settingAcs_url(Webs *wp);
void checkAcsEnable(Webs *wp);
void acs_enable(Webs *wp);
void acs_disable(Webs *wp);

/*settingNetMode*/
void checkNetMode(Webs *wp);
void set_mode_1(Webs *wp);
void set_mode_2(Webs *wp);
void set_snat(Webs *wp);
void set_dnat(Webs *wp);
void clear_nat(Webs *wp);
void query_nat(Webs *wp);

/*settingSecurity*/
void checkSecurityEnable(Webs *wp);
void startSecurity(Webs *wp);
void cancelSecurity(Webs *wp);
void getSecurityLog(Webs *wp);
void querySecurity(Webs *wp);
void settingSecurity(Webs *wp);
/*settingWH*/
void getRebootTime(Webs *wp);
void setRebootTime(Webs *wp);
void checkManualTime(Webs *wp);
void setManualTime(Webs *wp);
void setAutoTime(Webs *wp);
void queryAddr(Webs *wp);
void settingAddr(Webs *wp);
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
int get_config(char *package,char *section,char *option,char *buff); 
int set_config(char *package,char *section,char *option,char *value,int commit); 
#endif
#endif

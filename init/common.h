#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>

#define DATA_LEN	255 /* test data's len */
/*定时重启时间10分钟*/
#define REBOOT_TIME 600

int fd;
/*网络状态标志：1 有效；0 无效*/
int net_sta;
/*ecm拨号状态标志：1 已拨号；0 未拨号*/
int ecm_done;
/*系统时间设置标志：1 已设置；0 未设置*/
int setTime_done;

int getConfig(char *Config, char *buff, char *ConfigFile);
int get_config(char *package,char *section,char *option,char *buff);

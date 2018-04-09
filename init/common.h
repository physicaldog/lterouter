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

/*定时重启时间10分钟*/
#define REBOOT_TIME 600

int fd;
/*网络状态标志：1 有效；0 无效*/
int net_sta;
/*ecm拨号状态标志：1 已拨号；0 未拨号*/
int ecm_done;


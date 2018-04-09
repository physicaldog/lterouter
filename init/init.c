#include "common.h"
#include "at.h"

#define DATA_LEN	0xFF  /* test data's len */
#define SerPort	"/dev/ttyem300"
//#define SerPort	"/dev/ttyUSB2"

char at_arr[16][64] = {/*{{{*/
	"AT^SYSINFOEX\r\n", 
	"AT^NDISSTATQRY?\r\n",
	"AT^NDISDUP=1,1,\"cmnet\"\r\n",
	"AT^NDISDUP=1,0,",
};/*}}}*/

static int openDev(char *Dev)
{/*{{{*/
	int iFd;

	struct termios opt; 

	//iFd = open(Dev, O_RDWR | O_NOCTTY | O_NONBLOCK);                        
	iFd = open(Dev, O_RDWR | O_NOCTTY);                        
	if(iFd < 0) {
		syslog(LOG_DEBUG,"open %s failed!\n",Dev);
		return -1;
	}

	tcgetattr(iFd, &opt);      

	//cfsetispeed(&opt, B57600);
	//cfsetospeed(&opt, B57600);

	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);


	opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG | ECHOE);
	opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
	opt.c_oflag   &=   ~(OPOST);
	opt.c_cflag   &=   ~(CSIZE   |   PARENB);
	opt.c_cflag   |=   CS8;

	opt.c_cc[VMIN]   =   DATA_LEN;                                      
	opt.c_cc[VTIME]  =   150;

	if (tcsetattr(iFd,   TCSANOW,   &opt)<0) {
		return   -1;
	}


	return iFd;
}/*}}}*/

//多线程
void status_read(int *fptr)
{/*{{{*/
	char rbuff[4096] = {'\0'};
	char* ptr = rbuff;
	int ret=0;
	fd = *fptr;
	while (1) {
		ret = read(*fptr, ptr, 0x01);
		if (1 == ret) {
			if ('\n' != *ptr) {
				ptr++;
			}
			else {
				//printf("%s",rbuff);
				//syslog(LOG_DEBUG,"Get direct: %s\n",rbuff);
				direct_process(rbuff);
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
			}
		}
		else {
			syslog(LOG_DEBUG,"read err:\n");
			return;
		}
	}
	syslog(LOG_DEBUG,"read exit\n");
}/*}}}*/

void send_at(int *fptr)
{/*{{{*/
	int ret;
	/*循环检测，防止主动上报信息接受失败*/
	while(1){
		ret = write(*fptr, at_arr[0], strlen(at_arr[0]));
		if(ret < 0){
			syslog(LOG_DEBUG,"send_at\n");
			return;
		}
		/*时间间隔尽量大于拨号指令返回时长*/
		sleep(10);
	}
	return;

}/*}}}*/

void alarm_handler()
{
	syslog(LOG_DEBUG,"Alarm Reboot Device!!!\n");
	system("bash `reboot`");
}
int main(void) 
{
	int i=0,ret=0,fd=0;
	pthread_t rThread, tThread;
	
	/*添加系统日志*/
	openlog("lterouter", LOG_CONS | LOG_PID, LOG_LOCAL2);
	syslog(LOG_DEBUG,"This is lterouter syslog!\n");
	syslog(LOG_DEBUG,"*********************************************\n");
	
	/*初始化lanip*/
	lanInit();

	/*当设备附着不上网络时定时重启*/
	signal(SIGALRM,alarm_handler);
	alarm(REBOOT_TIME);

	/*打开模组串口*/
	fd = openDev(SerPort);
	if(0 >= fd){
		syslog(LOG_DEBUG,"OPEN /dev/ttyem300 FAILED!\n");
		return -1;
	}

	//        signal(SIGALRM, send_at);
	//        alarm(2);
	ret = pthread_create(&rThread,NULL,(void *)status_read,(void *)&fd);
	if(0 != ret){
		syslog(LOG_DEBUG,"rThread create failed!\n");
		return -1;
	}
	ret = pthread_create(&tThread,NULL,(void *)send_at,(void *)&fd);
	if(0 != ret){
		syslog(LOG_DEBUG,"tThread create failed!\n");
		return -1;
	}

	ret = pthread_join(rThread,NULL);
	ret = pthread_join(tThread,NULL);

	close(fd);

	return 0;
}

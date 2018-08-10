#include "common.h"
#include "at.h"

#define DATA_LEN	0xFF  /* test data's len */
#define SerPort	"/dev/ttyem302"
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
	log_msg("\n****%s****\n",__FUNCTION__);
	iFd = open(Dev, O_RDWR | O_NOCTTY);                        
	if(iFd < 0) {
		log_msg("open %s failed!\n",Dev);
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
	log_msg("\n****%s****\n",__FUNCTION__);
	while (1) {
		//log_msg("%s\n",__FUNCTION__);
		ret = read(*fptr, ptr, 0x01);
		if (1 == ret) {
			if ('\n' != *ptr) {
				ptr++;
			}
			else {
				log_msg("%s",rbuff);
				//log_msg("Get direct: %s\n",rbuff);
				direct_process(rbuff);
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
			}
		}
		else {
			log_msg("read err:\n");
			return;
		}
	}
	log_msg("read exit\n");
}/*}}}*/

void send_at(int *fptr)
{/*{{{*/
	int ret;
	/*循环检测，防止主动上报信息接受失败*/
	while(1){
		log_msg("\n****%s****\n",__FUNCTION__);
		ret = write(*fptr, at_arr[0], strlen(at_arr[0]));
		if(ret < 0){
			log_msg("send_at\n");
			return;
		}
		/*时间间隔尽量大于拨号指令返回时长*/
		sleep(10);
	}
	return;

}/*}}}*/

void alarm_handler()
{
	log_msg("\n********Alarm Reboot Device!!!*********\n");
	system("echo mv messages");
	system("cp /var/log/messages /opt/log/");
	system("reboot");
}
int main(void) 
{
	int i=0,ret=0,fd=0;
	pthread_t rThread, tThread;
	
	log_msg("\n********%s********\n",__FUNCTION__);

	/*添加系统日志*/
	log_open("suyi_lterouter", LOG_CONS | LOG_PID, LOG_LOCAL2);

	log_msg("This is lterouter log_msg!\n");
	log_msg("*********************************************\n");
	
	/*初始化lanip*/
	lanInit();

	/*当设备附着不上网络时定时重启*/
	signal(SIGALRM,alarm_handler);
	alarm(REBOOT_TIME);

	/*打开模组串口*/
	fd = openDev(SerPort);
	if(0 >= fd){
		log_msg("OPEN /dev/ttyem300 FAILED!\n");
		return -1;
	}

	//        signal(SIGALRM, send_at);
	//        alarm(2);
	ret = pthread_create(&rThread,NULL,(void *)status_read,(void *)&fd);
	if(0 != ret){
		log_msg("rThread create failed!\n");
		return -1;
	}
	ret = pthread_create(&tThread,NULL,(void *)send_at,(void *)&fd);
	if(0 != ret){
		log_msg("tThread create failed!\n");
		return -1;
	}

	ret = pthread_join(rThread,NULL);
	ret = pthread_join(tThread,NULL);

	close(fd);

	return 0;
}

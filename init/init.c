#include "common.h"
#include "at.h"
int reboottime=0;

#define DATA_LEN	0xFF  /* test data's len */
#define SerPort	"/dev/ttyem302"
//#define SerPort	"/dev/ttyUSB2"


static int openDev(char *Dev)
{/*{{{*/
	int iFd;

	struct termios opt; 

	//iFd = open(Dev, O_RDWR | O_NOCTTY | O_NONBLOCK);                        
	log_msg("\n****%s****\n",__FUNCTION__);
	iFd = open(Dev, O_RDWR | O_NOCTTY);                        
	if(iFd < 0) {
		log_msg("open %s failed!\n",Dev);
		log_syslog("open ttyem302 failed!","\n");
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
	char rbuff[1024] = {'\0'};
	char* ptr = rbuff;
	int ret=0;
	fd = *fptr;
	log_msg("\n****%s****\n",__FUNCTION__);
	while (1) {
		ret = read(*fptr, ptr, 0x01);
		if (1 == ret) {
			if ('\n' != *ptr) {
				ptr++;
			}
			else {
				//log_msg("AT Read: %s\n",rbuff);
				direct_process(rbuff);
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
			}
		}
		else {
			log_msg("AT Read err:\n");
			log_syslog("AT Read err","\n");
			return;
		}
	}
	log_msg("AT Read Exit\n");
	log_syslog("AT Read Exit","\n");
}/*}}}*/

char at_arr[16][64] = {/*{{{*/
	"AT^SYSINFOEX\r\n", 
	"AT^NDISSTATQRY?\r\n",
	"AT^NDISDUP=1,1,\"cmnet\"\r\n",
	"AT^NDISDUP=1,0,",
	"AT+CPIN?\r\n",
	"AT^MONSC\r\n",
	"AT+CIMI\r\n",
	"AT+CGSN\r\n",
	"AT^HCSQ?\r\n",
};/*}}}*/
int serial_write(int fd,char *buff)
{
	int ret = 0;	
	ret = write(fd, buff, strlen(buff));
	if(ret < 0){
		log_msg("AT Send %s Failed\n",buff);
		log_syslog("AT Send Failed","\n");
		return -1;
	}
	return 0;
}
void send_at(int *fptr)
{/*{{{*/
	int ret;
	char attech_stat[8] = {0};
	char ndis_stat[8] = {0};
	char uci_buff[32] = {0};
	char user_name_buff[32] = {0};
	char psword_buff[32] = {0};
	char at_buff[64] = {0};
	/*循环检测，防止主动上报信息接受失败*/
	while(1){
		//log_msg("\n****%s****\n",__FUNCTION__);
		ret = serial_write(*fptr,"AT^SYSINFOEX\r\n");
		if(ret < 0)
			return;
		sleep(2);

		get_config("status","module","attech_stat",attech_stat);
		if(!strcmp(attech_stat,"true")){
			ret = serial_write(*fptr,"AT^NDISSTATQRY?\r\n");
			if(ret < 0)
				return;
			sleep(5);
		}

		get_config("status","module","attech_stat",attech_stat);
		get_config("status","module","ndis_stat",ndis_stat);
		if((!strcmp(attech_stat,"true")) && (!strcmp(ndis_stat,"false"))){
			log_syslog("*****","激活网络链接*****\n");
			get_config("config","apn","apn",uci_buff);
			get_config("config","apn","lte_user_name",user_name_buff);
			get_config("config","apn","lte_psword",psword_buff);
			sprintf(at_buff,"AT^NDISDUP=1,1,\"%s\",\"%s\",\"%s\"\r\n",uci_buff,user_name_buff,psword_buff);
			ret = serial_write(*fptr,at_buff);
			if(ret < 0)
				return;
			memset(at_buff,0,strlen(at_buff));
			memset(uci_buff,0,strlen(uci_buff));
			memset(user_name_buff,0,strlen(user_name_buff));
			memset(psword_buff,0,strlen(psword_buff));
			sleep(5);
		}else if((!strcmp(attech_stat,"false")) && (!strcmp(ndis_stat,"true"))){
			log_syslog("*****","去激活网络链接*****\n");
			get_config("config","apn","apn",uci_buff);
			sprintf(at_buff,"AT^NDISDUP=1,0,\"%s\"\r\n",uci_buff);
			ret = serial_write(*fptr,at_buff);
			if(ret < 0)
				return;
			memset(at_buff,0,strlen(at_buff));
			memset(uci_buff,0,strlen(uci_buff));
			sleep(5);
		}

		get_config("status","module","ndis_stat",ndis_stat);
		if(!strcmp(ndis_stat,"true")){
			ret = serial_write(*fptr,"AT^HCSQ?\r\n");
			if(ret < 0)
				return;
			sleep(2);

			ret = serial_write(*fptr,"AT^MONSC\r\n");
			if(ret < 0)
				return;
			sleep(2);

			memset(uci_buff,0,strlen(uci_buff));
			get_config("status","module","imei",uci_buff);
			if(15 > strlen(uci_buff)){
				ret = serial_write(*fptr,"AT+CGSN\r\n");
				if(ret < 0)
					return;
				sleep(2);
			}
		}

		memset(uci_buff,0,strlen(uci_buff));
		ret = serial_write(*fptr,"AT+CIMI\r\n");
		if(ret < 0)
			return;
		sleep(2);
	}
	return;

}/*}}}*/

void alarm_handler()
{
	log_syslog("***************Offline TimeOut","Reboot Device!**********************\n");
	log_msg("***************Offline TimeOut","Reboot Device!**********************\n");
	system("/opt/init/Reboot.sh Offline Timeout;");
}
int main(void) 
{
	int i=0,ret=0,fd=0;
	char buff[64] = {0};
	pthread_t rThread, tThread;
	
	log_msg("\n********%s********\n",__FUNCTION__);

	/*添加系统日志*/
	log_open("suyi_lterouter", LOG_CONS | LOG_PID, LOG_LOCAL2);

	log_msg("This is lterouter log_msg!\n");
	log_msg("*********************************************\n");
	log_syslog("********************lterouter","start!!!********************\n");
	
	/*初始化lanip*/
	//lanInit();

	/*当设备附着不上网络时定时重启*/
	//getConfig("RebootTime",buff,"/opt/config/RebootTime");
	get_config("config","system","reboot_interval",buff);
	reboottime = atoi(buff);
	log_msg("reboottime:%d\n",reboottime);
	signal(SIGALRM,alarm_handler);
	alarm(reboottime);

	while(1){

		ret = access(SerPort,0);
		if(ret != 0){
			log_msg("NO %s!!!!!\n",SerPort);
			log_syslog("NO /dev/ttyem302!!!!","\n");
			sleep(5);
			continue;
		}
		/*打开模组串口*/
		fd = openDev(SerPort);
		if(0 >= fd){
			log_msg("OPEN /dev/ttyem302 FAILED!\n");
			log_syslog("OPEN /dev/ttyem302 FAILED!","\n");
			continue;
		}
		log_msg("OPEN /dev/ttyem302 OK fd = %d!!!!!!\n",fd);
		set_config("status","module","offline_timeout","false",1);

		ret = pthread_create(&rThread,NULL,(void *)status_read,(void *)&fd);
		if(0 != ret){
		close(fd);
			log_msg("rThread create failed!\n");
			log_syslog("rThread create failed!","\n");
			continue;
		}
		ret = pthread_create(&tThread,NULL,(void *)send_at,(void *)&fd);
		if(0 != ret){
			close(fd);
			log_msg("tThread create failed!\n");
			log_syslog("tThread create failed!","\n");
			continue;
		}
		log_msg("Thread create!!!!!!!!!!!!!!\n");
		ret = pthread_join(rThread,NULL);
		log_msg("rThread create ret = %d!\n",ret);
		ret = pthread_join(tThread,NULL);
		log_msg("tThread create ret = %d!\n",ret);
		close(fd);
	}
	
	/*打开模组串口
	fd = openDev(SerPort);
	if(0 >= fd){
		log_msg("OPEN /dev/ttyem300 FAILED!\n");
		log_syslog("OPEN /dev/ttyem300 FAILED!","\n");
		return -1;
	}

	ret = pthread_create(&rThread,NULL,(void *)status_read,(void *)&fd);
	if(0 != ret){
		log_msg("rThread create failed!\n");
		log_syslog("rThread create failed!","\n");
		return -1;
	}
	ret = pthread_create(&tThread,NULL,(void *)send_at,(void *)&fd);
	if(0 != ret){
		log_msg("tThread create failed!\n");
		log_syslog("tThread create failed!","\n");
		return -1;
	}

	ret = pthread_join(rThread,NULL);
	ret = pthread_join(tThread,NULL);

	close(fd);
*/
	return 0;
}

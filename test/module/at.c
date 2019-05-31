#include "common.h"
#include "at.h"

#define ApnConf "/opt/config/ApnConfig"

char *at[8] = {
	"AT^NDISDUP=1,1,",
	"AT^NDISSTATQRY?\r\n",
};

void direct_process(char *buff)
{
	char *ptr = NULL;
	char *at_arr[16] = {
		"SRVST",
		"NDISSTAT",
		"SYSINFOEX",
		"NDISSTATQRY",
	};
	log_msg("****%s****\n",__FUNCTION__);
	log_msg("buff:%s\n",buff);
	if ('^' == buff[0]) {
		if(strstr(buff,"AT^"))
			return;
		if ((strstr(buff,at_arr[0])) || (strstr(buff,at_arr[2]))) {
			get_sysinfoex(buff);
			return;
		}
		else if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
			get_ndisstat(buff);
			return;
		}
		else if(strstr(buff,"NWTIME")){
			get_nwtime(buff);
			return;
		}

	}
}

void get_nwtime(char *buff)
{
	int i = 0,ret = 0;
	char *ptr = NULL;
	char *qtr = NULL;
	char time[32] = {'\0'};
	char hour[4] = {'\0'};

	log_msg("\n****%s****\n",__FUNCTION__);
	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	ptr++;
	if('9' == *ptr){
		setTime_done = 0;
		return;
	}
	else{
		log_msg(time,"date -s \"20");
		for(i=0;i<2;i++){
			qtr = strchr(ptr,'/');
			if(qtr == NULL){
				log_msg("qtr failed\n");
				return;
			}
			log_msg("%c\n",*qtr);
			*qtr = '-';
		}
		qtr = strchr(ptr,',');
		*qtr = ' ';
		qtr = strchr(ptr,'+');
		memset(qtr,'\0',strlen(qtr));
		strcat(time,ptr);
		strcat(time,"\"");
		log_msg("%s\n",time);
		system(time);
		setTime_done = 1;
	}

}

void get_sysinfoex(char *buff)
{
	int i = 0;
	char *ptr = NULL;
	char *qtr = NULL;

	log_msg("****%s****%\n",__FUNCTION__);


	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	if(strstr(buff,"SRVST")){
		if (('2' != ptr[1])){
			log_msg("无效网络\n");
			log_msg("\n");
			net_sta = 0;
			ecm_done = 0;
			system("kill `cat /opt/udhcpc.pid`");
		}
	}
	else{
		qtr = strchr(buff,',');
		for(i=0;i<5;i++)
		{
			qtr = strchr(qtr,',');
			qtr++;
		}
		if (('2' == ptr[1]) && ('6' == *qtr)){
			log_msg("网络有效\n");
			alarm(REBOOT_TIME);

			/*网络有效，检查网络链接状态*/
			write(fd,at[1],strlen(at[1]));
			net_sta = 1;
		}
		else{
			log_msg("网络无效\n");
			log_msg("无效网络\n");
			log_msg("\n");
			net_sta = 0;
			system("kill `cat /opt/udhcpc.pid`");
			ecm_done = 0;
		}

	}
}

void get_ndisstat(char *buff)
{
	char *ptr = NULL;
	FILE *fe = NULL;
	int ret = 0;
	int file = 0;
	char ip[64] = {'\0'};
	char rbuff[64] = {'\0'};//读取文件缓存
	char direct[64] = {'\0'};//


	log_msg("\n****%s****\n",__FUNCTION__);
	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	switch (ptr[1]){
		case '0':
			log_msg("连接断开\n");
			ecm_done = 0;
			if((1 == net_sta)){
				log_msg("开始拨号\n");
				strcat(direct,"AT^NDISDUP=1,1,\"");//
				//从配置文件中获取apn;                   
				//getConfig("apn",rbuff,ApnConf);
				get_config(rbuff,"apn","apn","config");
				strcat(direct,rbuff);
				strcat(direct,"\"\r\n");
				log_msg("dir= %s\n",direct);

				write(fd,"\r\n",strlen("\r\n"));
				write(fd,direct,strlen(direct));
			}
			break;
		case '1'://获取ip
			//log_msg("有效连接\n");
			//log_msg("\n");
			if(0 == setTime_done)
				write(fd,"at^nwtime?\r\n",strlen("at^nwtime?\r\n"));
			if(0 == ecm_done){
				log_msg("获取ip\n");
				system("bash /opt/init/netconf.sh");
				ret = get_local_ip("usb0",ip);
				if(0 != strcmp(ip,"")){
					ecm_done = 1;
					log_msg("usb0 ip = %s\n",ip);
					log_msg("\n");
				}
				else{
					ecm_done = 0;
					log_msg("未获取到ip\n");
					log_msg("\n");
				}//如何检测系统的网络链接
			}
			//ecm_done = 0;
			break;
		case '2':
			break;
		case '3':
			break;
	}
}

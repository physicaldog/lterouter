#include "common.h"
#include "at.h"

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
	if ('^' == buff[0]) {
		//syslog(LOG_DEBUG,"Get direct:%s\n",buff);
		//ptr = strstr(buff,at_arr[0]);
		if ((strstr(buff,at_arr[0])) || (strstr(buff,at_arr[2]))) {
			get_sysinfoex(buff);
			return;
		}
		//ptr = strstr(buff,at_arr[1]);
		if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
			get_ndisstat(buff);
			return;
		}

	}
}

void get_sysinfoex(char *buff)
{
	int i = 0;
	char *ptr = NULL;
	char *qtr = NULL;

	ptr = strstr(buff," ");
	qtr = strchr(buff,',');
	for(i=0;i<5;i++)
	{
		qtr = strchr(qtr,',');
		qtr++;
	}
	if(NULL == ptr)
		return;
	//printf("网络有效,%c,%c\n",ptr[1],*qtr);
	if (('2' == ptr[1]) && ('6' == *qtr)){
//	if (('2' == ptr[1])){
		//syslog(LOG_DEBUG,"有效网络\n");
		//syslog(LOG_DEBUG,"\n");
		/*成功附着网络重置定时器*/
		alarm(REBOOT_TIME);

		/*网络有效，检查网络链接状态*/
		write(fd,at[1],strlen(at[1]));
		net_sta = 1;
	}
	else{
		syslog(LOG_DEBUG,"无效网络\n");
		syslog(LOG_DEBUG,"\n");
		net_sta = 0;
		ecm_done = 0;
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


	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	switch (ptr[1]){
		case '0':
			syslog(LOG_DEBUG,"连接断开\n");
			syslog(LOG_DEBUG,"\n");
			ecm_done = 0;
			if((1 == net_sta)){
				syslog(LOG_DEBUG,"开始拨号\n");
				strcat(direct,"AT^NDISDUP=1,1,\"");//
				//从配置文件中获取apn;                   
				fe = fopen("/opt/config/netconfig","r");
				if (NULL == fe){
					syslog(LOG_DEBUG,"netconfig open failed!\n");
					//return;
				}
				else{
					while(fgets(rbuff,sizeof(rbuff),fe)){
						if(strstr(rbuff,"apn:"))
						{   
							file = 1;
							break;
						}
						else
							memset(rbuff,'\0',sizeof(rbuff));
					}
					if(0 == file){
						syslog(LOG_DEBUG,"未配置apn\n");
						syslog(LOG_DEBUG,"\n");
					}
					fclose(fe);

					rbuff[strlen(rbuff)-1] = '\0';
					strcat(direct,rbuff+strlen("apn:"));
				}
				strcat(direct,"\"\r\n");
				syslog(LOG_DEBUG,"dir= %s\n",direct);

				write(fd,"\r\n",strlen("\r\n"));
				write(fd,direct,strlen(direct));
			}
			break;
		case '1'://获取ip
			//syslog(LOG_DEBUG,"有效连接\n");
			//syslog(LOG_DEBUG,"\n");
			if(0 == ecm_done){
				syslog(LOG_DEBUG,"获取ip\n");
				system("bash /opt/init/netconf.sh");
				ret = get_local_ip("usb0",ip);
				if(0 != strcmp(ip,"")){
					ecm_done = 1;
					syslog(LOG_DEBUG,"usb0 ip = %s\n",ip);
					syslog(LOG_DEBUG,"\n");
				}
				else{
					ecm_done = 0;
					syslog(LOG_DEBUG,"未获取到ip\n");
					syslog(LOG_DEBUG,"\n");
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

#include "system_common.h"
#define TTY_EM300	"/dev/ttyem300"
#define TTY_EM302	"/dev/ttyem300"

bool srvst = false;
bool stat = false;
bool first_ecm = true;

char at_arr[16][64] = {/*{{{*/
	"AT^SYSINFOEX\r\n", 
	"AT^NDISSTATQRY?\r\n",
	"AT^NDISDUP=1,1,\"cmnet\"\r\n",
	"AT^NDISDUP=1,0,",
	"AT^HCSQ?\r\n",
};/*}}}*/

int get_hcsq(char *buff)
{
	int i=0;
	char sysmode[8] = {0};
	char rssib[32] = {0};
	char rsrpb[32] = {0};
	char sinrb[32] = {0};
	char rsrqb[32] = {0};
	char *ptr = NULL;
	char *qtr = NULL;
	
	printf("%****%s****%\n",__FUNCTION__);
	ptr = strstr(buff,"LTE");
	if(NULL == ptr)
		return 0;
	ptr = strstr(buff,"\n");
	*ptr = '\0';
	ptr = strchr(buff,',');
	strcpy(rssib,(++ptr));

	ptr = strchr(rssib,',');
	strcpy(rsrpb,(++ptr));

	ptr = strchr(rsrpb,',');
	strcpy(sinrb,(++ptr));

	ptr = strchr(sinrb,',');
	strcpy(rsrqb,(++ptr));
	
	qtr = strchr(rssib,',');
	*qtr = '\0';

	qtr = strchr(rsrpb,',');
	*qtr = '\0';
	
	qtr = strchr(sinrb,',');
	*qtr = '\0';
	
//	rssi = atoi(rssib);
//	rsrp = atoi(rsrpb);
//	sinr = atoi(sinrb);
//	rsrq = atoi(rsrqb);
	set_config("status","module","rssi",rssib,false);
	set_config("status","module","rsrp",rsrpb,false);
	set_config("status","module","sinr",sinrb,false);
	set_config("status","module","rsrq",rsrqb,false);
	printf("rssi=%s rsrp=%s sinr=%s rsrq=%s\n",rssib,rsrpb,sinrb,rsrqb);
	return 0;
}
void get_sysinfoex(char *buff)
{
	int i = 0;
	char *ptr = NULL;
	char *qtr = NULL;
	char rbuff[8] = {0};

	printf("****%s****%\n",__FUNCTION__);

	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	if(strstr(buff,"SRVST")){
		sprintf(rbuff,"%c",ptr[1]);
		set_config("status","module","srvst",rbuff,false);
		if (('2' != ptr[1])){
			log_msg("网络无效\n");
			net_sta = 0;
			ecm_done = 0;
		}
	}
	else{
		memset(rbuff,0,sizeof(rbuff));
		sprintf(rbuff,"%c",ptr[1]);
		set_config("status","module","srvst",rbuff,false);

		qtr = strchr(buff,',');
		for(i=0;i<3;i++){
			qtr = strchr(qtr,',');
			qtr++;
		}
		memset(rbuff,0,sizeof(rbuff));
		sprintf(rbuff,"%c",*qtr);
		set_config("status","module","simst",rbuff,false);

		qtr = strchr(buff,',');
		for(i=0;i<5;i++)
		{
			qtr = strchr(qtr,',');
			qtr++;
		}
		if (('2' == ptr[1]) && ('6' == *qtr)){
			log_msg("网络有效\n");
			net_sta = 1;
		}
		else{
			log_msg("网络无效\n");
			net_sta = 0;
			ecm_done = 0;
		}

		ptr = strchr(buff,"\"");
		ptr++;
		qtr = strchr(buff,"\"");
		*qtr = '\0';
		memset(rbuff,0,sizeof(rbuff));
		sprintf(rbuff,"%s",ptr);
		set_config("status","module","mode",rbuff,false);



	}
}

void get_ndisstat(char *buff)
{
	int ret = 0;
	char *ptr = NULL; 
	char rbuff[8] = {0};//读取文件缓存


	printf("\n****%s****\n",__FUNCTION__);
	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	sprintf(rbuff,"%c",ptr[1]);
	set_config("status","module","stat",rbuff,false);
	
	}
}

int direct_process(char *buff)
{
	char *ptr = NULL;
	char *at_arr[16] = {
		"SRVST",
		"NDISSTAT",
		"SYSINFOEX",
		"NDISSTATQRY",
		"CSQ",
		"HCSQ",
	};
	//printf("\n********%s********\n",__FUNCTION__);

	if(strstr(buff,"ERROR"))
		return 0;
	
	if (strstr(buff,"+CGSN:")) {
		//get_cgsn(buff);
		return 0;
	}
	if (strstr(buff,"HCSQ")) {
		get_hcsq(buff);
		return 0;
	}
	if ('^' == buff[0]) {
		if(strstr(buff,"AT^"))
			return;
		//printf("Get direct:%s\n",buff);
		if ((strstr(buff,at_arr[0])) || (strstr(buff,at_arr[2]))) {
			get_sysinfoex(buff);
			return 0;
		}
		if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
			get_ndisstat(buff);
			return 0;
		}
		if(strstr(buff,"NWTIME")){
			//get_nwtime(buff);
			return 0;
		}
	}
}
//多线程
void tty_read(int *fptr)
{/*{{{*/
	char rbuff[4096] = {'\0'};
	char* ptr = rbuff;
	int ret=0;

	printf("\n****%s****\n",__FUNCTION__);
	while (1) {
		//printf("%s\n",__FUNCTION__);
		ret = read(*fptr, ptr, 0x01);
		if (1 == ret) {
			if ('\n' != *ptr) {
				ptr++;
			}
			else {
				//printf("%s",rbuff);
				printf("Get direct: %s\n",rbuff);
				direct_process(rbuff);
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
			}
		}
		else {
			printf("read err:\n");
			return;
		}
	}
	printf("read exit\n");
}/*}}}*/

void tty300_send(int *fptr)
{/*{{{*/
	int ret;
	/*循环检测，防止主动上报信息接受失败*/
	while(1){
		printf("\n****%s****\n",__FUNCTION__);
		ret = write(*fptr, at_arr[4], strlen(at_arr[4]));
		if(ret < 0){
			printf("send_at\n");
			return;
		}
		/*时间间隔尽量大于拨号指令返回时长*/
		sleep(10);
	}
	return;

}/*}}}*/

void tty302_send(int *fptr)
{/*{{{*/
	int ret;
	/*循环检测，防止主动上报信息接受失败*/
	while(1){
		printf("\n****%s****\n",__FUNCTION__);
		ret = write(*fptr, at_arr[0], strlen(at_arr[0]));
		if(ret < 0){
			printf("send_at\n");
			return;
		}
		sleep(2);
		ret = write(*fptr, at_arr[1], strlen(at_arr[1]));
		/*时间间隔尽量大于拨号指令返回时长*/
		sleep(10);
	}
	return;

}/*}}}*/

int main(int argc,char *argv[]) 
{
	int i=0,ret=0;

	pthread_t rThread_300, tThread_300, rThread_302, tThread_302;
	printf("\n********%s********\n",__FUNCTION__);
	
	/*打开模组串口*/
	tty300_fd = openDev(TTY_EM300);
	if(0 >= tty300_fd){
		printf("OPEN /dev/ttyem300 FAILED!\n");
		return -1;
	}

	tty302_fd = openDev(TTY_EM302);
	if(0 >= tty302_fd){
		printf("OPEN /dev/ttyem302 FAILED!\n");
		return -1;
	}

	ret = pthread_create(&rThread_300,NULL,(void *)tty_read,(void *)&tty300_fd);
	if(0 != ret){
		printf("rThread_300 create failed!\n");
		return -1;
	}
	ret = pthread_create(&tThread_300,NULL,(void *)tty300_send,(void *)&tty300_fd);
	if(0 != ret){
		printf("tThread_300 create failed!\n");
		return -1;
	}

	ret = pthread_create(&rThread_302,NULL,(void *)tty_read,(void *)&tty302_fd);
	if(0 != ret){
		printf("rThread_302 create failed!\n");
		return -1;
	}
	ret = pthread_create(&tThread_302,NULL,(void *)tty302_send,(void *)&tty302_fd);
	if(0 != ret){
		printf("tThread_302 create failed!\n");
		return -1;
	}

	ret = pthread_join(rThread_300,NULL);
	ret = pthread_join(tThread_300,NULL);
	ret = pthread_join(rThread_302,NULL);
	ret = pthread_join(tThread_302,NULL);

	close(tty300_fd);
	close(tty302_fd);

	return 0;
}



#include "common.h"
#include "at.h"
#include<time.h> 
extern int reboottime;

#define ApnConf "/opt/config/ApnConfig"
#define SYSLOG "/opt/log/syslog"
int old_attech_stat = 3;
int old_ndis_stat = 2;
int old_sim_stat = 2;

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
		"MONSC",
		"CIMI",
		"HCSQ",
		"CPIN",
		"CGSN",	
		"SIMST"
	};
	log_msg("***%s Get Buff:%s***\n",__FUNCTION__,buff);
	if ('^' == buff[0]) {
		if(strstr(buff,"AT^"))
			return ;
		if ((strstr(buff,at_arr[0])) || (strstr(buff,at_arr[2]))) {
			get_sysinfoex(buff);
			return;
		}
		else if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
			get_ndisstat(buff);
			return;
		}
		else if(strstr(buff,"NWTIME")){
			//get_nwtime(buff);
			return;
		}
		else if(strstr(buff,at_arr[4])) {
			get_monsc(buff);
			return;
		}
		else if(strstr(buff,at_arr[5])) {
			get_cimi(buff);
			return;
		}	
		else if(strstr(buff,at_arr[6])) {
			get_hcsq(buff);
			return;
		}
		else if(strstr(buff,at_arr[9])) {
			get_simst(buff);
			return;
		}

	}
	else if ('+' == buff[0]) {
			if(strstr(buff,at_arr[7])) {
				//get_cpin(buff);
				return;
			}
			else if(strstr(buff,at_arr[8])) {
				get_cgsn(buff);
				return;
			}
			
	}
	else if(0 == strncmp(buff, "460", 3)) {
		get_cimi(buff);
		return;
	}
}

void get_cgsn(char *buff)
{
	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	char IMEI[16] = {'\0'};

	//char uci_cmd[128] = {'\0'};

	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = strchr(buff, '\"');
	if(!ptr)
		return;
	ptr++;
	qtr = strchr(ptr, '\"');
	if(!qtr)
		return;
	memset(IMEI, 0x0, sizeof(IMEI));
	strncpy(IMEI, ptr, qtr-ptr);
	log_msg("IMEI:%s\n",IMEI);
	set_config("status","module","imei",IMEI,1);

	//memset(uci_cmd, 0x0, sizeof(uci_cmd));
	//snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.parameter.serial_number=%s", IMEI);
	//system(uci_cmd);

}

void get_cpin(char *buff)
{
	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	char simcard_state[10] = {'\0'};

	char uci_cmd[128] = {'\0'};

	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = strchr(buff, ':');
	if(!ptr)
		return;
	ptr = ptr+2;
	qtr = strchr(ptr, '\r');
	if(!qtr)
		return;
	memset(simcard_state, 0x0, sizeof(simcard_state));
	strncpy(simcard_state, ptr, qtr-ptr);

	memset(uci_cmd, 0x0, sizeof(uci_cmd));
	snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.simcard.state=%s", simcard_state);
	system(uci_cmd);
	//log_msg("\n**%s**\n", uci_cmd);		
}

void get_hcsq(char * buff)
{
	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	
	char lte_stat[8] = {'\0'};
	char rbuff[64] = {0};
	int rssi = 0;
	int rsrp = 0;
	double sinr = 0.0;
	double rsrq = 0.0;

	//char uci_cmd[128] = {'\0'};
	
	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = strstr(buff, "LTE");
	if(NULL == ptr) 
	{
		return;	
	}
	else
	{
		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');		
		if(!qtr)
			return;
		strncpy(lte_stat, ptr, qtr-ptr);
		rssi = atoi(lte_stat) - 121;
		
		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');		
		if(!qtr)
			return;
		memset(lte_stat,0,8);
		strncpy(lte_stat, ptr, qtr-ptr);
		rsrp = atoi(lte_stat) - 141;

		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');
		if(!qtr)
			return;
		memset(lte_stat,0,8);
		strncpy(lte_stat, ptr, qtr-ptr);
		sinr = atoi(lte_stat)*0.2 - 20.2;

		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, '\r');
		if(!qtr)
			return;
		memset(lte_stat,0,8);
		strncpy(lte_stat, ptr, qtr-ptr);
		rsrq = atoi(lte_stat)*0.5 - 20;
		sprintf(rbuff,"rssi:%d rsrp:%d sinr:%.1f rsrq:%.1f \n",rssi,rsrp,sinr,rsrq);
		log_msg("HCSQ:%s",rbuff);

		memset(lte_stat,0,8);
		sprintf(lte_stat,"%d",rssi);
		set_config("status","module","rssi",lte_stat,1);
		memset(lte_stat,0,8);
		sprintf(lte_stat,"%d",rsrp);
		set_config("status","module","rsrp",lte_stat,1);
		memset(lte_stat,0,8);
		sprintf(lte_stat,"%.1f",sinr);
		set_config("status","module","sinr",lte_stat,1);
		memset(lte_stat,0,8);
		sprintf(lte_stat,"%.1f",rsrq);
		set_config("status","module","rsrq",lte_stat,1);

		//memset(uci_cmd, 0x0, sizeof(uci_cmd));
		//snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.SINR=%.1f", (-20.2+0.2*sinr));
		//system(uci_cmd);
		//log_msg("\n**%s**\n", uci_cmd);
	}
}

void get_cimi(char *buff)
{
	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	char IMSI[16] = {'\0'};

	//char uci_cmd[128] = {'\0'};

	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = buff;
	if(!ptr)
		return;
	qtr = strchr(ptr, '\r');
	if(!qtr)
		return;
	memset(IMSI, 0x0, sizeof(IMSI));
	strncpy(IMSI, ptr, qtr-ptr);
	set_config("status","module","imsi",IMSI,1);

	//memset(uci_cmd, 0x0, sizeof(uci_cmd));
	//snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.identity.imsi=%s", IMSI);
	//system(uci_cmd);
	//log_msg("\n**%s**\n", uci_cmd);	
	
}

void get_monsc(char *buff)
{

	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	char MCC[8] = {'\0'};
	char MNC[8] = {'\0'};
	char ARFCN[8] = {'\0'};
	char CellID[10] = {'\0'};
	char PCI[8] = {'\0'};
	char TAC[8] = {'\0'};
	char RSRP[8] = {'\0'};
	char RSRQ[8] = {'\0'};
	char RXLEV[8] = {'\0'};

	//char uci_cmd[128] = {'\0'};
	
	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = strstr(buff, "LTE");
	if(NULL == ptr) 
	{
		return;	
	}
	else
	{
		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');		
		if(!qtr)
			return;
		memset(MCC, 0x0, sizeof(MCC));
		strncpy(MCC, ptr, qtr-ptr);
		set_config("status","module","mcc",MCC,1);
		
		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');		
		if(!qtr)
			return;
		memset(MNC, 0x0, sizeof(MNC));
		strncpy(MNC, ptr, qtr-ptr);
		set_config("status","module","mnc",MNC,1);

		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');
		if(!qtr)
			return;
		memset(ARFCN, 0x0, sizeof(ARFCN));
		strncpy(ARFCN, ptr, qtr-ptr);

		ptr = strchr(ptr, ',');
		if(!ptr)
			return;
		ptr++;
		qtr = strchr(ptr, ',');
		if(!qtr)
			return;
		memset(CellID, 0x0, sizeof(CellID));
		strncpy(CellID, ptr, qtr-ptr);
		set_config("status","module","cellid",CellID,1);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(PCI, 0x0, sizeof(PCI));
		strncpy(PCI, ptr, qtr-ptr);
		set_config("status","module","pci",PCI,1);

/*
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(TAC, 0x0, sizeof(TAC));
		strncpy(TAC, ptr, qtr-ptr);
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(RSRP, 0x0, sizeof(RSRP));
		strncpy(RSRP, ptr, qtr-ptr);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(RSRQ, 0x0, sizeof(RSRQ));
		strncpy(RSRQ, ptr, qtr-ptr);
		printf("rsrp:%s,rsrq:%s\n",RSRP,RSRQ);
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, '\r');
		memset(RXLEV, 0x0, sizeof(RXLEV));
		strncpy(RXLEV, ptr, qtr-ptr);	

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.MCC=%s", MCC);
		system(uci_cmd);
	//	log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.MNC=%s", MNC);
		system(uci_cmd);
	//	log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.CellID=%s", CellID);
		system(uci_cmd);
	//	log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.PCI=%s", PCI);
		system(uci_cmd);
	//	log_msg("\n**%s**\n", uci_cmd);

	//	memset(uci_cmd, 0x0, sizeof(uci_cmd));
	//	snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.RSRP0=%s", RSRP);
	//	system(uci_cmd);
	//	log_msg("\n**%s**\n", uci_cmd);

*/
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

void lte_sim(int new_sim_stat, char *buff)
{
	int ret = 0;
	if(new_sim_stat == old_sim_stat){
		//状态相同
	}
	else{
		if(new_sim_stat){
			log_syslog("SIM卡有效",buff);
			set_config("status","module","sim_stat","true",1);
			old_sim_stat = 1;
		}else{
			log_syslog("SIM卡无效",buff);
			set_config("status","module","sim_stat","false",1);
			old_sim_stat = 0;
		}
	}
	return;
}

void get_simst(char *buff)
{
	int i = 0,ret = 0;
	char *ptr = NULL;

	log_msg("\n****%s****\n",__FUNCTION__);
	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	ptr++;
	if('1' == *ptr){
		lte_sim(1,buff);
	}
	else{
		lte_sim(0,buff);
	}

}

void log_syslog(char *log,char *buff)
{
	char logbuff[512] = {0};
	char timebuff[32] = {0};
	time_t t;//将t声明为时间变量
	struct tm *p;//struct tm是一个结构体，声明一个结构体指针
	FILE *fp = NULL;
	pid_t pid = getpid();

	time(&t);

	if(!buff)
		return;
	p=localtime(&t);//获得当地的时间
	sprintf(timebuff,"%d-%d-%d %d:%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	sprintf(logbuff,"%-20s lterouter pid %-8d:%s,%s\n",timebuff,pid,log,buff);

	fp = fopen(SYSLOG,"a+");
	if(!fp){
		log_msg("/opt/log/syslog open failed \n!!!");
		return;
	}

	fwrite(logbuff,sizeof(char),strlen(logbuff),fp);
	fclose(fp);
	return;
}

void lte_attech(int new_attech_stat, char *buff)
{
	int ret = 0;
	if(new_attech_stat == old_attech_stat){
		//状态相同
	}
	else{
		if(new_attech_stat){
			log_syslog("已附着",buff);
			set_config("status","module","attech_stat","true",1);
			old_attech_stat = 1;
		}else{
			log_syslog("未附着",buff);
			system("kill -9 `cat /opt/tmp/udhcpc.pid`");
			set_config("status","module","attech_stat","false",1);
			old_attech_stat = 0;
		}
	}
	return;
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
			log_msg("网络无效\n");
			lte_attech(0,buff);
		}
	}
	else{
		qtr = strchr(buff,',');
		if(NULL == qtr)
			return;
		for(i=0;i<5;i++)
		{
			qtr = strchr(qtr,',');
			if(NULL == qtr)
				return;
			qtr++;
		}
		//if (('2' == ptr[1]) && ('6' == *qtr)){
		if (('2' == ptr[1])){
			log_msg("网络有效\n");
			lte_attech(1,buff);

		}
		else{
			log_msg("网络无效\n");
			lte_attech(0,buff);
		}

		if('6' == *qtr)
			set_config("status","module","netmode","LTE",0);
		else if('1' == *qtr)
			set_config("status","module","netmode","GSM",0);
		else if('3' == *qtr)
			set_config("status","module","netmode","WCDMA",0);
		else if('4' == *qtr)
			set_config("status","module","netmode","D_SCDMA",0);


	}
}

void lte_ndis(int new_ndis_stat, char *buff)
{
	int ret = 0;
	char ip[64] = {'\0'};
	if(new_ndis_stat == old_ndis_stat){
		if(new_ndis_stat){
			alarm(reboottime);
			ret = get_local_ip("usb0",ip);
			if(0 == strlen(ip)){
				log_msg("未获取到ip,重新获取\n");
				system("/opt/init/netconf.sh");
			}
		}
	}
	else{
		if(new_ndis_stat){
			log_syslog("已拨号",buff);
			set_config("status","module","ndis_stat","true",1);
			old_ndis_stat = 1;
			system("/opt/init/netconf.sh");
			system("uci -c /opt/config set status.system.online_time=`date +\%Y\%m\%d-\%H:\%M:\%S`;uci -c /opt/config commit status.system");
		}else{
			log_syslog("未拨号",buff);
			set_config("status","module","ndis_stat","false",1);
			old_ndis_stat = 0;
			system("uci -c /opt/config set status.system.offline_time=`date +\%Y\%m\%d-\%H:\%M:\%S`;uci -c /opt/config commit status.system");
		}
	}
	return;
}
void get_ndisstat(char *buff)
{
	char *ptr = NULL;
	int ret = 0;


	log_msg("\n****%s****\n",__FUNCTION__);
	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	switch (ptr[1]){
		case '0':
			log_msg("未连接\n");
			lte_ndis(0,buff);
			break;
		case '1'://获取ip
			log_msg("有效连接\n");
			lte_ndis(1,buff);
			break;
		case '2':
			break;
		case '3':
			break;
	}
}

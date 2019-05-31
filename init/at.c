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
		"MONSC",
		"CIMI",
		"HCSQ",
		"CPIN",
		"CGSN",	
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
		else if(strstr(buff,at_arr[4])) {
			get_monsc(buff);
			return;
		}
/*		else if(strstr(buff,at_arr[5])) {
			get_cimi(buff);
			return;
		}
*/		
		else if(strstr(buff,at_arr[6])) {
			get_hcsq(buff);
			return;
		}

	}
	else if ('+' == buff[0]) {
			if(strstr(buff,at_arr[7])) {
				get_cpin(buff);
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

	char uci_cmd[128] = {'\0'};

	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = strchr(buff, '\"');
	ptr++;
	qtr = strchr(ptr, '\"');
	memset(IMEI, 0x0, sizeof(IMEI));
	strncpy(IMEI, ptr, qtr-ptr);

	memset(uci_cmd, 0x0, sizeof(uci_cmd));
	snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.parameter.serial_number=%s", IMEI);
	system(uci_cmd);
	log_msg("\n**%s**\n", uci_cmd);
/*	
	memset(uci_cmd, 0x0, sizeof(uci_cmd));
	snprint(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config commit tr069");
	system(uci_cmd);
	log_msg("\n**%s**\n", uci_cmd);
*/

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
	ptr = ptr+2;
	qtr = strchr(ptr, '\r');
	memset(simcard_state, 0x0, sizeof(simcard_state));
	strncpy(simcard_state, ptr, qtr-ptr);

	memset(uci_cmd, 0x0, sizeof(uci_cmd));
	snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.simcard.state=%s", simcard_state);
	system(uci_cmd);
	log_msg("\n**%s**\n", uci_cmd);		
}

void get_hcsq(char * buff)
{
	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	
	char lte_rssi[8] = {'\0'};
	char lte_rsrp[8] = {'\0'};
	char lte_sinr[8] = {'\0'};
	char lte_rsrq[8] = {'\0'};
	int sinr = 0;

	char uci_cmd[128] = {'\0'};
	
	log_msg("\n****%s****\n",__FUNCTION__);


	ptr = strstr(buff, "LTE");
	if(NULL == ptr) 
	{
		return;	
	}
	else
	{
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');		
		memset(lte_rssi, 0x0, sizeof(lte_rssi));
		strncpy(lte_rssi, ptr, qtr-ptr);
		
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');		
		memset(lte_rsrp, 0x0, sizeof(lte_rsrp));
		strncpy(lte_rsrp, ptr, qtr-ptr);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(lte_sinr, 0x0, sizeof(lte_sinr));
		strncpy(lte_sinr, ptr, qtr-ptr);
		sinr = atoi(lte_sinr);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, '\r');
		memset(lte_rsrq, 0x0, sizeof(lte_rsrq));
		strncpy(lte_rsrq, ptr, qtr-ptr);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.SINR=%.1f", (-20.2+0.2*sinr));
		system(uci_cmd);
		log_msg("\n**%s**\n", uci_cmd);
	}

}






void get_cimi(char *buff)
{
	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	char IMSI[16] = {'\0'};

	char uci_cmd[128] = {'\0'};

	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = buff;
	qtr = strchr(ptr, '\r');
	memset(IMSI, 0x0, sizeof(IMSI));
	strncpy(IMSI, ptr, qtr-ptr);

	memset(uci_cmd, 0x0, sizeof(uci_cmd));
	snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.identity.imsi=%s", IMSI);
	system(uci_cmd);
	log_msg("\n**%s**\n", uci_cmd);	
	
}

void get_monsc(char *buff)
{

	int i=0,ret=0;
	char *ptr = NULL;
	char *qtr = NULL;
	char MCC[5] = {'\0'};
	char MNC[5] = {'\0'};
	char ARFCN[8] = {'\0'};
	char CellID[10] = {'\0'};
	char PCI[5] = {'\0'};
	char TAC[6] = {'\0'};
	char RSRP[5] = {'\0'};
	char RSRQ[5] = {'\0'};
	char RXLEV[5] = {'\0'};

	char uci_cmd[128] = {'\0'};
	
	log_msg("\n****%s****\n",__FUNCTION__);

	ptr = strstr(buff, "LTE");
	if(NULL == ptr) 
	{
		return;	
	}
	else
	{
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');		
		memset(MCC, 0x0, sizeof(MCC));
		strncpy(MCC, ptr, qtr-ptr);
		
		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');		
		memset(MNC, 0x0, sizeof(MNC));
		strncpy(MNC, ptr, qtr-ptr);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(ARFCN, 0x0, sizeof(ARFCN));
		strncpy(ARFCN, ptr, qtr-ptr);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(CellID, 0x0, sizeof(CellID));
		strncpy(CellID, ptr, qtr-ptr);

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, ',');
		memset(PCI, 0x0, sizeof(PCI));
		strncpy(PCI, ptr, qtr-ptr);

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

		ptr = strchr(ptr, ',');
		ptr++;
		qtr = strchr(ptr, '\r');
		memset(RXLEV, 0x0, sizeof(RXLEV));
		strncpy(RXLEV, ptr, qtr-ptr);	

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.MCC=%s", MCC);
		system(uci_cmd);
		log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.MNC=%s", MNC);
		system(uci_cmd);
		log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.CellID=%s", CellID);
		system(uci_cmd);
		log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.PCI=%s", PCI);
		system(uci_cmd);
		log_msg("\n**%s**\n", uci_cmd);

		memset(uci_cmd, 0x0, sizeof(uci_cmd));
		snprintf(uci_cmd, sizeof(uci_cmd)-1, "uci -c /opt/config set tr069.radio.RSRP0=%s", RSRP);
		system(uci_cmd);
		log_msg("\n**%s**\n", uci_cmd);

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

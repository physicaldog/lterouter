#include"suyi_common.h"
#define ApnConf "/opt/config/ApnConfig"
#define Version "/opt/config/Version"
char netmode;
char netStatus;
char simStatus;
char ecmStatus;
char csq[4] = {'\0'};
int  rssi = 0;
int  rsrp = 0;
int  sinr = 0;
int  rsrq = 0;
int  AutoTime = 0;


void sysInfo(webs_t wp, char_t *path, char_t *query)
{
	char timeStr[64] = {0};
	//websWrite(wp,T("%s"),buff);
	struct timeval uptime;
	printf("\n********%s********\n",__FUNCTION__);


	uptime = get_uptime();
	
	printf("uptime = %lu\n", uptime.tv_sec);
	//get_timeStr(uptime.tv_sec,timeStr);
	//printf("timeStr:%s\n",timeStr);
	websWrite(wp,T("{"));
	websWrite(wp,T("\"uptime\":\"%lu\""),uptime.tv_sec);

	websWrite(wp,T("}"));
	websDone(wp,200);
	return;
}

void deviceInfo(webs_t wp, char_t *path, char_t *query)
{
    char pName[32] = {'\0'};
    char pModle[32] = {'\0'};
    char sVer[32] = {'\0'};
	printf("\n********%s********\n",__FUNCTION__);
    getConfig("productName",pName,Version);
    getConfig("productModle",pModle,Version);
    getConfig("softVersion",sVer,Version);
    printf("pName:%s,pModle:%s,sVer:%s\n",pName,pModle,sVer);

    websWrite(wp,T("{"));
    websWrite(wp,T("\"pName\":\"%s\","),pName);
    websWrite(wp,T("\"pModle\":\"%s\","),pModle);
    websWrite(wp,T("\"sVer\":\"%s\""),sVer);
    websWrite(wp,T("}"));

	websDone(wp,200);
	return;
}

void WANStatus(webs_t wp, char_t *path, char_t *query)
{

	int fd = 0;
	int ret = 0;
	char *name;
	char sinrb[8] = {0};
	char rsrqb[8] = {0};
	char buff[64] = {'\0'};//读取文件缓存
	char ip[32] = {'\0'};//读取文件缓存
	char netmask[32] = {'\0'};//读取文件缓存
	char macaddr[32] = {'\0'};//读取文件缓存
	char *system_date[64] = {0};
	FILE *fp;
	char *ptr = NULL;
	printf("\n********%s********\n",__FUNCTION__);

	fp = popen("date +\"%Y-%m-%d %Hh:%Mm\"","r");
	fread(system_date,1,sizeof(system_date),fp);
	fclose(fp);

	if(NULL != (ptr = strchr(system_date,'\n')))
		*ptr = 0;
	printf("system_date:%s\n",system_date);

	fd = openDev(SerPort);
	if(0 >= fd){
		printf("serPort open failed!\n");
		websWrite(wp,T("{"));
		websWrite(wp,T("\"system_date\":\"%s\","),system_date);
		websWrite(wp,T("\"netStatus\":\"设备异常！\""));
		websWrite(wp,T("}"));
		websDone(wp,200);
		return;

	}
	tcflush(fd,TCIFLUSH);
	printf("fd=%d\n",fd);
	//getSysinfo(fd);
	at_send(fd,"at^sysinfoex\r\n");
	at_read(fd);
	printf("sysinfoex\n");

	tcflush(fd,TCIFLUSH);
	at_send(fd,"at^ndisstatqry?\r\n");
	at_read(fd);
	printf("ndis\n");

	tcflush(fd,TCIFLUSH);
	//at_send(fd,"at+csq\r\n");
	at_send(fd,"at^hcsq?\r\n");
	printf("send_csq\n");
	at_read(fd);
	printf("csq\n");

	getConfig("apn",buff,ApnConf);
	printf("apn:%s\n",buff);
	get_local_ip(WAN_NAME,ip,netmask,macaddr);
	printf("wanip:%s,netmask:%s,macaddr:%s\n",ip,netmask,macaddr);

	websWrite(wp,T("{"));
	if('6' == netmode)
		websWrite(wp,T("\"netmode\":\"LTE\","));
	else if('1' == netmode)
		websWrite(wp,T("\"netmode\":\"GSM\","));
	else if('3' == netmode)
		websWrite(wp,T("\"netmode\":\"WCDMA\","));
	else if('4' == netmode)
		websWrite(wp,T("\"netmode\":\"TD_SCDMA\","));

	if('2' == netStatus)
		websWrite(wp,T("\"netStatus\":\"成功附着\","));
	else
		websWrite(wp,T("\"netStatus\":\"附着失败\","));
	if('1' == simStatus)
		websWrite(wp,T("\"simStatus\":\"sim有效\","));
	else
		websWrite(wp,T("\"simStatus\":\"sim无效\","));
	if('1' == ecmStatus)	
		websWrite(wp,T("\"ecmStatus\":\"已连接\","));
	else
		websWrite(wp,T("\"ecmStatus\":\"未连接\","));
	if((0 == rssi) || (99 == rssi))		
		websWrite(wp,T("\"rssi\":\"信号不可測\","));
	else
		websWrite(wp,T("\"rssi\":\"%d dBm\","),(-121+rssi));

	if((0 == rsrp) || (255 == rsrp))		
		websWrite(wp,T("\"rsrp\":\"信号不可測\","));
	else
		websWrite(wp,T("\"rsrp\":\"%d dBm\","),(-140+rsrp));

	if((0 == sinr) || (99 == sinr))		
		websWrite(wp,T("\"sinr\":\"信号不可測\","));
	else{
		sprintf(sinrb,"%.1f",(-20.2+0.2*sinr));
		websWrite(wp,T("\"sinr\":\"%s dB\","),sinrb);
	}
	if((0 == rsrq) || (255 == rsrq))		
		websWrite(wp,T("\"rsrq\":\"信号不可測\","));
	else{
		sprintf(rsrqb,"%.1f",(-20.0+0.5*rsrq));
		websWrite(wp,T("\"rsrq\":\"%s dBm\","),rsrqb);
	}
	websWrite(wp,T("\"system_date\":\"%s\","),system_date);
	websWrite(wp,T("\"apn\":\"%s\","),buff);
	websWrite(wp,T("\"wanip\":\"%s\","),ip);
	websWrite(wp,T("\"wanmask\":\"%s\","),netmask);
	websWrite(wp,T("\"wanmac\":\"%s\""),macaddr);
	websWrite(wp,T("}"));
	rssi = 0;
	printf("websdone!\n");
	websDone(wp,200);

	ret = access("/opt/config/ManualTime",0);
	if(0 == ret){
		AutoTime = 0;
	}else{
		if(0 == AutoTime){
			tcflush(fd,TCIFLUSH);
			at_send(fd,"at^nwtime?\r\n");
			at_read(fd);
			AutoTime = 1;
		}else{

		}
	}


	close(fd);
	return;
}

void LANStatus(webs_t wp, char_t *path, char_t *query)
{
	char ip[32] = {'\0'};//读取文件缓存
	char netmask[32] = {'\0'};//读取文件缓存
	char macaddr[32] = {'\0'};//读取文件缓存
	printf("LANStatus\n");
	get_local_ip(LAN_NAME,ip,netmask,macaddr);
	printf("wanip:%s,netmask:%s,macaddr:%s\n",ip,netmask,macaddr);
	websWrite(wp,T("{"));
	websWrite(wp,T("\"lanip\":\"%s\","),ip);
	websWrite(wp,T("\"lanmask\":\"%s\","),netmask);
	websWrite(wp,T("\"lanmac\":\"%s\""),macaddr);
	websWrite(wp,T("}"));

	websDone(wp,200);
	return;
}

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
int  Autoime = 0;


void sysInfo(Webs *wp)
{
	char timeStr[64] = {0};
	//websWrite(wp,("%s"),buff);
	struct timeval uptime;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);


	uptime = get_uptime();
	
	printf("uptime = %lu\n", uptime.tv_sec);
	//get_timeStr(uptime.tv_sec,timeStr);
	//printf("timeStr:%s\n",timeStr);
	websWrite(wp,("{"));
	websWrite(wp,("\"uptime\":\"%lu\""),uptime.tv_sec);

	websWrite(wp,("}"));
	websDone(wp);
	return;
}

void deviceInfo(Webs *wp)
{
    char pName[32] = {'\0'};
    char pModle[32] = {'\0'};
    char sVer[32] = {'\0'};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
    getConfig("productName",pName,Version);
    getConfig("productModle",pModle,Version);
    getConfig("softVersion",sVer,Version);
    printf("pName:%s,pModle:%s,sVer:%s\n",pName,pModle,sVer);

    websWrite(wp,("{"));
    websWrite(wp,("\"pName\":\"%s\","),pName);
    websWrite(wp,("\"pModle\":\"%s\","),pModle);
    websWrite(wp,("\"sVer\":\"%s\""),sVer);
    websWrite(wp,("}"));

	websDone(wp);
	return;
}

void WANStatus(Webs *wp)
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
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	fp = popen("date +\"%Y-%m-%d %Hh:%Mm\"","r");
	fread(system_date,1,sizeof(system_date),fp);
	fclose(fp);

	if(NULL != (ptr = strchr(system_date,'\n')))
		*ptr = 0;
	printf("system_date:%s\n",system_date);

	fd = openDev(SerPort);
	if(0 >= fd){
		printf("serPort open failed!\n");
		websWrite(wp,("{"));
		websWrite(wp,("\"system_date\":\"%s\","),system_date);
		websWrite(wp,("\"netStatus\":\"设备异常！\""));
		websWrite(wp,("}"));
		websDone(wp);
		return;

	}
	tcflush(fd,TCIOFLUSH);
	printf("fd=%d\n",fd);
	//getSysinfo(fd);
	at_send(fd,"at^sysinfoex\r\n");
	at_read(fd);
	printf("sysinfoex\n");

	tcflush(fd,TCIOFLUSH);
	at_send(fd,"at^ndisstatqry?\r\n");
	at_read(fd);
	printf("ndis\n");

	tcflush(fd,TCIOFLUSH);
	//at_send(fd,"at+csq\r\n");
	at_send(fd,"at^hcsq?\r\n");
	printf("send_csq\n");
	at_read(fd);
	printf("csq\n");

	getConfig("apn",buff,ApnConf);
	printf("apn:%s\n",buff);
	get_local_ip(WAN_NAME,ip,netmask,macaddr);
	printf("wanip:%s,netmask:%s,macaddr:%s\n",ip,netmask,macaddr);

	websWrite(wp,("{"));
	if('6' == netmode)
		websWrite(wp,("\"netmode\":\"LTE\","));
	else if('1' == netmode)
		websWrite(wp,("\"netmode\":\"GSM\","));
	else if('3' == netmode)
		websWrite(wp,("\"netmode\":\"WCDMA\","));
	else if('4' == netmode)
		websWrite(wp,("\"netmode\":\"D_SCDMA\","));

	if('2' == netStatus)
		websWrite(wp,("\"netStatus\":\"成功附着\","));
	else
		websWrite(wp,("\"netStatus\":\"附着失败\","));
	if('1' == simStatus)
		websWrite(wp,("\"simStatus\":\"sim有效\","));
	else
		websWrite(wp,("\"simStatus\":\"sim无效\","));
	if('1' == ecmStatus)	
		websWrite(wp,("\"ecmStatus\":\"已连接\","));
	else
		websWrite(wp,("\"ecmStatus\":\"未连接\","));
	if((0 == rssi) || (99 == rssi))		
		websWrite(wp,("\"rssi\":\"信号不可測\","));
	else
		websWrite(wp,("\"rssi\":\"%d dBm\","),(-121+rssi));

	if((0 == rsrp) || (255 == rsrp))		
		websWrite(wp,("\"rsrp\":\"信号不可測\","));
	else
		websWrite(wp,("\"rsrp\":\"%d dBm\","),(-140+rsrp));

	if((0 == sinr) || (99 == sinr))		
		websWrite(wp,("\"sinr\":\"信号不可測\","));
	else{
		sprintf(sinrb,"%.1f",(-20.2+0.2*sinr));
		websWrite(wp,("\"sinr\":\"%s dB\","),sinrb);
	}
	if((0 == rsrq) || (255 == rsrq))		
		websWrite(wp,("\"rsrq\":\"信号不可測\","));
	else{
		sprintf(rsrqb,"%.1f",(-20.0+0.5*rsrq));
		websWrite(wp,("\"rsrq\":\"%s dBm\","),rsrqb);
	}
	websWrite(wp,("\"system_date\":\"%s\","),system_date);
	websWrite(wp,("\"apn\":\"%s\","),buff);
	websWrite(wp,("\"wanip\":\"%s\","),ip);
	websWrite(wp,("\"wanmask\":\"%s\","),netmask);
	websWrite(wp,("\"wanmac\":\"%s\""),macaddr);
	websWrite(wp,("}"));
	rssi = 0;
	printf("websdone!\n");
	websDone(wp);

	ret = access("/opt/config/Manualime",0);
	if(0 == ret){
		Autoime = 0;
	}else{
		if(0 == Autoime){
			tcflush(fd,TCIOFLUSH);
			at_send(fd,"at^nwtime?\r\n");
			at_read(fd);
			Autoime = 1;
		}else{

		}
	}


	close(fd);
	return;
}

void LANStatus(Webs *wp)
{
	char ip[32] = {'\0'};//读取文件缓存
	char netmask[32] = {'\0'};//读取文件缓存
	char macaddr[32] = {'\0'};//读取文件缓存
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	get_local_ip(LAN_NAME,ip,netmask,macaddr);
	printf("wanip:%s,netmask:%s,macaddr:%s\n",ip,netmask,macaddr);
	websWrite(wp,("{"));
	websWrite(wp,("\"lanip\":\"%s\","),ip);
	websWrite(wp,("\"lanmask\":\"%s\","),netmask);
	websWrite(wp,("\"lanmac\":\"%s\""),macaddr);
	websWrite(wp,("}"));

	websDone(wp);
	return;
}

#include"suyi_common.h"
#define ApnConf "/opt/config/ApnConfig"
#define Version "/opt/config/Version"
char netmode;
char netStatus;
char simStatus;
char ecmStatus;
char csq[4] = {'\0'};
int  rssi = 0;


void sysInfo(webs_t wp, char_t *path, char_t *query)
{
	char timeStr[64] = {'\0'};
	//websWrite(wp,T("%s"),buff);
	struct timeval uptime;
	printf("sysInfo:\n");

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
	printf("Deviceinfo:\n");
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
	char *name;
	char buff[64] = {'\0'};//读取文件缓存
	char ip[32] = {'\0'};//读取文件缓存
	char netmask[32] = {'\0'};//读取文件缓存
	char macaddr[32] = {'\0'};//读取文件缓存
	printf("CPUUsageRate:\n");
	fd = openDev(SerPort);

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
	at_send(fd,"at+csq\r\n");
	printf("send_csq\n");
	at_read(fd);
	printf("csq\n");
	close(fd);

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
		websWrite(wp,T("\"csq\":\"信号不可測\","));
	else
		websWrite(wp,T("\"csq\":\"%ddBm\","),(-113+2*rssi));
		
	websWrite(wp,T("\"apn\":\"%s\","),buff);
	websWrite(wp,T("\"wanip\":\"%s\","),ip);
	websWrite(wp,T("\"wanmask\":\"%s\","),netmask);
	websWrite(wp,T("\"wanmac\":\"%s\""),macaddr);
	websWrite(wp,T("}"));
	rssi = 0;
	printf("websdone!\n");

	websDone(wp,200);
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

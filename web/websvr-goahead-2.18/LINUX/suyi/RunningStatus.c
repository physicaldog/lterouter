#include"suyi_common.h"
char netmode;
char netStatus;
char simStatus;
char ecmStatus;
char csq[4] = {'\0'};
int  rssi = 0;


void sysInfo(webs_t wp, char_t *path, char_t *query)
{
	//char buff[] = {""}
	//websWrite(wp,T("%s"),buff);
	printf("sysinfo:\n");
	websWrite(wp,T("monkey"));

	websDone(wp,200);
	return;
}

void deviceInfo(webs_t wp, char_t *path, char_t *query)
{
	printf("deviceinfo:\n");
	websWrite(wp,T("{\"product_Name\":\"Customer Premise Equipment\","));
	websWrite(wp,T("\"product_Modle\":\"suyi_CPE\","));
	websWrite(wp,T("\"hw_ver\":\"CPE_V1.0\","));
	websWrite(wp,T("\"fw_ver\":\"CPE_V1.0\"}"));

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
	printf("sysinfo\n");

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

	getConfig("apn:",buff);
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

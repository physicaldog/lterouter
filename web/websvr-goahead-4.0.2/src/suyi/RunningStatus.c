#include"suyi_common.h"
#define ApnConf "/opt/config/ApnConfig"
#define Version "/opt/config/Version"
#define ADDRESS "/opt/config/ADDRESS"
char netmode;
char netStatus;
char simStatus;
char ecmStatus;
char csq[4] = {'\0'};
int  rssi = 0;
int  rsrp = 0;
int  sinr = 0;
int  rsrq = 0;
char  imei[128] = {'\0'};
int  AutoTime = 0;


void sysInfo(Webs *wp)
{
	char timeStr[64] = {0};
	//websWrite(wp,("%s"),buff);
	struct timeval uptime;
    char city_name[16] = {0};
    char county_name[16] = {0};
	char buff[128] = {0};
    char *location_name = buff;
	char *ptr = buff;
	//FILE *fp = NULL;
	//cJSON *Addr = NULL;

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	uptime = get_uptime();
	
	//printf("uptime = %lu\n", uptime.tv_sec);
	//get_timeStr(uptime.tv_sec,timeStr);
	//printf("timeStr:%s\n",timeStr);
	/*
	fp = fopen(ADDRESS,"r");
	if(NULL == fp){
		printf("文件打开失败\n");
	}else{
		fread(buff,1,sizeof(buff),fp);
		fclose(fp);
		Addr = cJSON_Parse(buff);
		if(NULL == Addr){
			cJSON_GetErrorPtr();
			websWrite(wp,("查询失败"));
		}else{
			city_name = cJSON_GetObjectItem(Addr,"city_name")->valuestring;
			county_name = cJSON_GetObjectItem(Addr,"county_name")->valuestring;
			location_name = cJSON_GetObjectItem(Addr,"location_name")->valuestring;

			websWrite(wp,("{"));
			websWrite(wp,("\"system_addr\":\"%s.%s.%s\","),city_name,county_name,location_name);
			websWrite(wp,("\"uptime\":\"%lu\""),uptime.tv_sec);
			websWrite(wp,("}"));

			cJSON_Delete(Addr);
		}
	}
*/
	get_config("config","addr","city",city_name);
	get_config("config","addr","county",county_name);
	get_config("config","addr","location",location_name);
	get_config("config","addr","install_time",buff);
	printf("install_time:%s\n",buff);

	websWrite(wp,("{"));
	websWrite(wp,("\"install_time\":\"%s\","),buff);
	//websWrite(wp,("\"install_time\":\"%s,"),buff);
	websWrite(wp,("\"install_addr\":\"%s.%s.%s\","),city_name,county_name,location_name);

	memset(buff,0,128);
	get_config("status","system","online_time",ptr);
	websWrite(wp,("\"online_time\":\"%s\","),ptr);

	memset(buff,0,128);
	get_config("status","system","offline_time",ptr);
	websWrite(wp,("\"offline_time\":\"%s\","),ptr);

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
    //getConfig("productName",pName,Version);
    //getConfig("productModle",pModle,Version);
    //getConfig("softVersion",sVer,Version);
    get_config("config","system","productName",pName);
    get_config("config","system","productModle",pModle);
    get_config("config","system","sVersion",sVer);
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

	FILE *fp;
	int ret = 0;
	char *ptr = NULL;
	char uci_buff[64] = {'\0'};//读取文件缓存

	char ip[32] = {'\0'};//读取文件缓存
	char netmask[32] = {'\0'};//读取文件缓存
	char macaddr[32] = {'\0'};//读取文件缓存

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	fp = popen("date +\"%Y-%m-%d %Hh:%Mm\"","r");
	fread(uci_buff,1,sizeof(uci_buff),fp);
	fclose(fp);

	if(NULL != (ptr = strchr(uci_buff,'\n')))
		*ptr = 0;
	//printf("system_date:%s\n",system_date);
	ret = access("/dev/ttyem300",0);
	if( 0 != ret){
		websWrite(wp,("{"));
		websWrite(wp,("\"system_date\":\"%s\","),uci_buff);
		websWrite(wp,("\"netStatus\":\"设备异常！\""));
		websWrite(wp,("}"));
		websDone(wp);
		return;
	}

	get_local_ip(WAN_NAME,ip,netmask,macaddr);
	printf("wanip:%s,netmask:%s,macaddr:%s\n",ip,netmask,macaddr);

	websWrite(wp,("{"));

	websWrite(wp,("\"system_date\":\"%s\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","netmode",uci_buff);
	websWrite(wp,("\"netmode\":\"%s\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","attech_stat",uci_buff);
	if(!strcmp(uci_buff,"true"))
		websWrite(wp,("\"netStatus\":\"成功附着\","));
	else
		websWrite(wp,("\"netStatus\":\"附着失败\","));

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","sim_stat",uci_buff);
	if(!strcmp(uci_buff,"true"))
		websWrite(wp,("\"simStatus\":\"sim有效\","));
	else
		websWrite(wp,("\"simStatus\":\"sim无效\","));

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","ndis_stat",uci_buff);
	if(!strcmp(uci_buff,"true"))
		websWrite(wp,("\"ecmStatus\":\"已连接\","));
	else
		websWrite(wp,("\"ecmStatus\":\"未连接\","));


	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","rssi",uci_buff);
	printf("%s\n",uci_buff);
	websWrite(wp,("\"rssi\":\"%s dBm\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","rsrp",uci_buff);
	printf("%s\n",uci_buff);
	websWrite(wp,("\"rsrp\":\"%s dBm\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","sinr",uci_buff);
	printf("%s\n",uci_buff);
	websWrite(wp,("\"sinr\":\"%s dB\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","rsrq",uci_buff);
	printf("%s\n",uci_buff);
	websWrite(wp,("\"rsrq\":\"%s dBm\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("config","apn","apn",uci_buff);
	websWrite(wp,("\"apn\":\"%s\","),uci_buff);

	memset(uci_buff,0,strlen(uci_buff));
	get_config("status","module","imei",uci_buff);
	websWrite(wp,("\"imei\":\"%s\","),uci_buff);

	websWrite(wp,("\"wanip\":\"%s\","),ip);
	websWrite(wp,("\"wanmask\":\"%s\","),netmask);
	websWrite(wp,("\"wanmac\":\"%s\""),macaddr);

	websWrite(wp,("}"));

	websDone(wp);
	return;
}

void WANStatus_bak(Webs *wp)
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
	at_send(fd,"at+cgsn\r\n");
	at_read(fd);
	printf("cgsn\n");

	tcflush(fd,TCIOFLUSH);
	//at_send(fd,"at+csq\r\n");
	at_send(fd,"at^hcsq?\r\n");
	at_read(fd);
	printf("hcsq\n");

	//getConfig("apn",buff,ApnConf);
	get_config("config","apn","apn",buff);
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
	websWrite(wp,("\"imei\":\"%s\","),imei);
	websWrite(wp,("\"wanip\":\"%s\","),ip);
	websWrite(wp,("\"wanmask\":\"%s\","),netmask);
	websWrite(wp,("\"wanmac\":\"%s\""),macaddr);
	websWrite(wp,("}"));
	rssi = 0;
	printf("websdone!\n");
	websDone(wp);

	ret = access("/opt/config/ManualTime",0);
	if(0 == ret){
		AutoTime = 0;
	}else{
		if(0 == AutoTime){
			tcflush(fd,TCIOFLUSH);
			at_send(fd,"at^nwtime?\r\n");
			at_read(fd);
			AutoTime = 1;
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

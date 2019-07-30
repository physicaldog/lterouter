#include"suyi_common.h"
#define LanConf "/opt/config/LanConfig"

void queryLAN(Webs *wp)
{
    char lanip[32] = {'\0'};
    char netmask[32] = {'\0'};

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
    //getConfig("lanip",lanip,LanConf);
    //getConfig("netmask",netmask,LanConf);
    get_config("config","lan","ip",lanip);
    get_config("config","lan","netmask",netmask);
    printf("lanip = %s\n",lanip);
    printf("netmask = %s\n",netmask);

    websWrite(wp,("{"));
    websWrite(wp,("\"lanip\":\"%s\","),lanip);
    websWrite(wp,("\"netmask\":\"%s\""),netmask);
    websWrite(wp,("}"));

    websDone(wp);
	return;
}

void setcontent(FILE *fd,char *Config, char *content)
{
	fwrite(Config,sizeof(char),strlen(Config),fd);
	fwrite(" ",sizeof(char),1,fd);
	fwrite(content,sizeof(char),strlen(content),fd);
	fwrite("\n",sizeof(char),1,fd);
	
}
int setDhcpConf(char *ip)
{
	FILE *fd = NULL;
	char *p = NULL;
	char ip_str[32] = {'\0'};
	int ret = 0;

	fd = fopen("/opt/config/udhcpd.conf","w+");
	if (NULL == fd){
		printf("no udhcpd.conf file\n");
		return -1;
	}
	strcpy(ip_str,ip);
	p = strrchr(ip_str,'.');
	ret = atoi(++p);
	sprintf(p,"%d",++ret);
	setcontent(fd,"start",ip_str);
	p = strrchr(ip_str,'.');
	strcpy(++p,"254\n");
	setcontent(fd,"end",ip_str);
	setcontent(fd,"option subnet","255.255.255.0");
	setcontent(fd,"opt router",ip);
	setcontent(fd,"interface","eth0");
	setcontent(fd,"max_leases","24");

	fclose(fd);

}
void settingLAN(Webs *wp)
{
	char *lanip;
	char *netmask;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	lanip = websGetVar(wp,("lanip"),("192.168.1.1"));
	netmask= websGetVar(wp,("netmask"),("255.255.255.0"));
	printf("lanip=%s\n",lanip);
	printf("netmask=%s\n",netmask);

	//setConfig("lanip",lanip,LanConf);
	//setConfig("netmask",netmask,LanConf);
    set_config("config","lan","ip",lanip,TRUE);
    set_config("config","lan","netmask",netmask,TRUE);
	setDhcpConf(lanip);

    websWrite(wp,("重启后生效"));
	websDone(wp);
	return;
}

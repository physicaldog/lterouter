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
    getConfig("lanip",lanip,LanConf);
    getConfig("netmask",netmask,LanConf);
    printf("lanip = %s\n",lanip);
    printf("netmask = %s\n",netmask);

    websWrite(wp,("{"));
    websWrite(wp,("\"lanip\":\"%s\","),lanip);
    websWrite(wp,("\"netmask\":\"%s\""),netmask);
    websWrite(wp,("}"));

    websDone(wp);
	return;
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

	setConfig("lanip",lanip,LanConf);
	setConfig("netmask",netmask,LanConf);

    websWrite(wp,("重启后生效"));
	websDone(wp);
	return;
}

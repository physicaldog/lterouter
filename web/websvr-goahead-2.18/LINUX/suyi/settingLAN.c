#include"suyi_common.h"
#define LanConf "/opt/config/LanConfig"

void queryLAN(webs_t wp, char_t *path, char_t *query)
{
    char lanip[32] = {'\0'};
    char netmask[32] = {'\0'};

    printf("lanip query\n");
    getConfig("lanip:",lanip,LanConf);
    getConfig("netmask:",netmask,LanConf);
    printf("lanip = %s\n",lanip);
    printf("netmask = %s\n",netmask);

    websWrite(wp,T("{"));
    websWrite(wp,T("\"lanip\":\"%s\","),lanip);
    websWrite(wp,T("\"netmask\":\"%s\""),netmask);
    websWrite(wp,T("}"));

    websDone(wp,200);
	return;
}

void settingLAN(webs_t wp, char_t *path, char_t *query)
{
	char *lanip;
	char *netmask;
	lanip = websGetVar(wp,T("lanip"),T("192.168.1.1"));
	netmask= websGetVar(wp,T("netmask"),T("255.255.255.0"));
	printf("lanip=%s\n",lanip);
	printf("netmask=%s\n",netmask);

	setConfig("lanip:",lanip,LanConf);
	setConfig("netmask:",netmask,LanConf);
	//set_ip("eth0",lanip,"255.255.255.0");

    websWrite(wp,T("重启后生效"));
	websDone(wp,200);
	return;
}

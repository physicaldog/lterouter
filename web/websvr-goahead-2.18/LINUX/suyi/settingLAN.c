#include"suyi_common.h"

void queryLAN(webs_t wp, char_t *path, char_t *query)
{
    char buff[64] = {'\0'};

    printf("lanip query\n");
    getConfig("lanip:",buff);
    printf("lan = %s\n",buff);

    websWrite(wp,T("%s"),buff);

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

	setConfig("lanip:",lanip);
	setConfig("netmask:",netmask);
	//set_ip("eth0",lanip,"255.255.255.0");

    websWrite(wp,T("重启后生效"));
	websDone(wp,200);
	return;
}

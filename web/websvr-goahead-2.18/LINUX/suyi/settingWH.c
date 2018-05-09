#include"suyi_common.h"

void resetDev(webs_t wp, char_t *path, char_t *query)
{
	printf("resetDev!\n");
    websWrite(wp,T("设备重启中..."));
	websDone(wp,200);
	/*系统重启*/
	sync();
	reboot(RB_AUTOBOOT);
	return;
}

void sysReset(webs_t wp, char_t *path, char_t *query)
{
	printf("sysReset!\n");
	system("cd /opt/upgrade;tar -xf config.tar -C /opt/");
    websWrite(wp,T("已恢复出厂设置"));
	websDone(wp,200);
	return;
}

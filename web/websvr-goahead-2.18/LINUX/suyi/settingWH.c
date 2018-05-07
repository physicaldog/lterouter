#include"suyi_common.h"

void resetDev(webs_t wp, char_t *path, char_t *query)
{
	printf("resetDev!\n");
	websDone(wp,200);
	/*系统重启*/
	sync();
	reboot(RB_AUTOBOOT);
	return;
}

void sysReset(webs_t wp, char_t *path, char_t *query)
{
	printf("sysReset!\n");
	system("cd /opt/upgrade;tar -xf system.tar -C /opt/");
	websDone(wp,200);
	return;
}

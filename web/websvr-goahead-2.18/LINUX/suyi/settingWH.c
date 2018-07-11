#include"suyi_common.h"

void queryTime(webs_t wp, char_t *path, char_t *query)
{

	int ret = 0;
	char *ptr = NULL;
	FILE *fp = NULL;
    char system_time[64] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	
	fp = popen("date +\"%Y-%m-%d %H:%M:%S\"","r");
	if(fp){
		ret = fread(system_time,1,sizeof(system_time),fp);
		if(NULL != (ptr = strchr(system_time,'\n')))
			*ptr = 0;
		fclose(fp);
	}
	printf("system_time=%s\n",system_time);
	

    websWrite(wp,T("{"));
    websWrite(wp,T("\"system_time\":\"%s\""),system_time);
    websWrite(wp,T("}"));

    websDone(wp,200);
	return;
}

void settingTime(webs_t wp, char_t *path, char_t *query)
{
    char *system_time;
	char command[64] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	system_time = websGetVar(wp,T("system_time"),T(""));
	
	printf("system_time=%s\n",system_time);
	if(strlen(system_time)){
		strcat(command,"date -s \"");
		strcat(command,system_time);
		strcat(command,"\"");

		system(command);
	}

    websWrite(wp,T("设置完成"));
	websDone(wp,200);
	return;
}

void setManualTime(webs_t wp, char_t *path, char_t *query)
{
	printf("\n********%s********\n",__FUNCTION__);
	system("touch /opt/config/ManualTime");

    websWrite(wp,T("设置完成"));
	websDone(wp,200);
	return;

}

void setAutoTime(webs_t wp, char_t *path, char_t *query)
{
	printf("\n********%s********\n",__FUNCTION__);
	system("rm /opt/config/ManualTime");

    websWrite(wp,T("设置完成"));
	websDone(wp,200);
	return;
}

void checkManualTime(webs_t wp, char_t *path, char_t *query)
{
	int ret = 0; 
    printf("\n********%s********\n",__FUNCTION__);
    websWrite(wp,T("{"));
	ret = access("/opt/config/ManualTime",0);
	if(-1 == ret){
		websWrite(wp,T("\"value\":\"0\""));
	}else{
		websWrite(wp,T("\"value\":\"1\""));
	}
    websWrite(wp,T("}"));
	websDone(wp,200);
	return;
}

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

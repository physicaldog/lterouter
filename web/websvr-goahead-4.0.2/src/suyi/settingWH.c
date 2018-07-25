#include"suyi_common.h"

void queryTime(Webs *wp)
{

	int ret = 0;
	char *ptr = NULL;
	FILE *fp = NULL;
    char system_time[64] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	
	fp = popen("date +\"%Y-%m-%d %H:%M:%S\"","r");
	if(fp){
		ret = fread(system_time,1,sizeof(system_time),fp);
		if(NULL != (ptr = strchr(system_time,'\n')))
			*ptr = 0;
		fclose(fp);
	}
	printf("system_time=%s\n",system_time);
	

    websWrite(wp,("{"));
    websWrite(wp,("\"system_time\":\"%s\""),system_time);
    websWrite(wp,("}"));

    websDone(wp);
	return;
}

void settingTime(Webs *wp)
{
    char *system_time;
	char command[64] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	system_time = websGetVar(wp,("system_time"),(""));
	
	printf("system_time=%s\n",system_time);
	if(strlen(system_time)){
		strcat(command,"date -s \"");
		strcat(command,system_time);
		strcat(command,"\"");

		system(command);
	}

    websWrite(wp,("设置完成"));
	websDone(wp);
	return;
}

void setManualTime(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	system("touch /opt/tmp/ManualTime");

    websWrite(wp,("设置完成"));
	websDone(wp);
	return;

}

void setAutoTime(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	system("rm /opt/tmp/ManualTime");

    websWrite(wp,("设置完成"));
	websDone(wp);
	return;
}

void checkManualTime(Webs *wp)
{
	int ret = 0; 
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
    websWrite(wp,("{"));
	ret = access("/opt/tmp/ManualTime",0);
	if(-1 == ret){
		websWrite(wp,("\"value\":\"0\""));
	}else{
		websWrite(wp,("\"value\":\"1\""));
	}
    websWrite(wp,("}"));
	websDone(wp);
	return;
}

void getRebootCount(Webs *wp)
{
	int ret = 0; 
	FILE *fp = NULL;
	char *ptr = NULL;
	char count[32] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	fp = popen("cat /opt/config/RebootCount | wc -l" ,"r");
	if(fp){
		fread(count,1,sizeof(count),fp);
		if(NULL != (ptr = strchr(count,'\n')))
			*ptr = 0;
		fclose(fp);
	}
	printf("count=%s\n",count);

    websWrite(wp,("设备启动次数：%s"),count);
	websDone(wp);
	return;
}

void resetDev(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);


    websWrite(wp,(" 设备重启中..."));
	websDone(wp);
	/*系统重启*/
	sync();
	reboot(RB_AUTOBOOT);
	return;
}

void sysReset(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	system("cd /opt/upgrade;tar -xf config.tar -C /opt/");
    websWrite(wp,("已恢复出厂设置"));
	websDone(wp);
	return;
}

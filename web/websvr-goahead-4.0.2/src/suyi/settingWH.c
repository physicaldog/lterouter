#include"suyi_common.h"
#define RebootTime "/opt/config/RebootTime"
#define ADDRESS "/opt/config/ADDRESS"

void queryAddr(Webs *wp)
{

	int ret = 0;
    char city_name[16];
    char county_name[16];
    char location_name[64];
    char longitude[64];
    char latitude[64];
    char businesstype[64];

    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	
	get_config("config","addr","city",city_name);
	get_config("config","addr","county",county_name);
	get_config("config","addr","location",location_name);

	get_config("status","system","longitude",longitude);
	get_config("status","system","latitude",latitude);
	get_config("status","system","businesstype",businesstype);

	websWrite(wp,("{"));
	websWrite(wp,("\"city_name\":\"%s\","),city_name);
	websWrite(wp,("\"county_name\":\"%s\","),county_name);
	websWrite(wp,("\"location_name\":\"%s\","),location_name);
	websWrite(wp,("\"longitude\":\"%s\","),longitude);
	websWrite(wp,("\"latitude\":\"%s\","),latitude);
	websWrite(wp,("\"businesstype\":\"%s\""),businesstype);
	websWrite(wp,("}"));

    websDone(wp);
	return;
}

void settingAddr(Webs *wp)
{
    char *ptr;
    char *city_name;
    char *county_name;
    char *location_name;
    char *longitude;
    char *latitude;
    char *businesstype;
	char buff[64] = {0};
	FILE *fp = NULL;
	int ret = 0;
	cJSON *Addr = NULL;
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	city_name = websGetVar(wp,("city_name"),(""));
	county_name = websGetVar(wp,("county_name"),(""));
	location_name = websGetVar(wp,("location_name"),(""));
	longitude = websGetVar(wp,("longitude"),(""));
	latitude = websGetVar(wp,("latitude"),(""));
	businesstype = websGetVar(wp,("businesstype"),(""));

	fp = popen("date +\"%Y-%m-%d %H:%M:%S\"","r");
	if(fp){
		ret = fread(buff,1,sizeof(buff),fp);
		if(NULL != (ptr = strchr(buff,'\n')))
			*ptr = 0;
		fclose(fp);
	}

	set_config("config","addr","city",city_name,1);
	set_config("config","addr","county",county_name,1);
	set_config("config","addr","location",location_name,1);
	//set_config("config","addr","install_time",buff,1);
	set_config("status","system","longitude",longitude,1);
	set_config("status","system","latitude",latitude,1);
	set_config("status","system","businesstype",businesstype,1);
	
	websDone(wp);
	return;
}

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
		system("hwclock -w");
	}

    websWrite(wp,("设置完成"));
	websDone(wp);
	return;
}

void getRebootTime(Webs *wp)
{
	int ret = 0; 
	FILE *fp = NULL;
	char *ptr = NULL;
	char buff[32] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	

	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	//getConfig("RebootTime",buff,RebootTime);
	get_config("config","system","reboot_interval",buff);

    websWrite(wp,("%s"),buff);
	websDone(wp);
	return;
}

void setRebootTime(Webs *wp)
{
    char *time;
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	time = websGetVar(wp,("time"),(""));
	
	//setConfig("RebootTime",time,RebootTime);
	set_config("config","system","reboot_interval",time,TRUE);

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
	char count[64] = {0};
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
/*
	fp = popen("cat /opt/config/RebootCount | wc -l" ,"r");
	if(fp){
		fread(count,1,sizeof(count),fp);
		if(NULL != (ptr = strchr(count,'\n')))
			*ptr = 0;
		fclose(fp);
	}
	*/
	get_config("config","system","reboot_count",count);
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
	//reboot(RB_AUTOBOOT);
	system("/opt/init/Reboot.sh \"**************webPage reboot!*************\"");
	return;
}

void sysReset(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	system("cd /opt/upgrade;tar -xf config.tar -C /opt/");
	sleep(1);
    websWrite(wp,("已恢复出厂设置"));
	websDone(wp);
	return;
}

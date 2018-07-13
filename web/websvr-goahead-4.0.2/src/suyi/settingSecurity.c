#include"suyi_common.h"
#define SecurityConf "/opt/security/naripcaccess.conf"
#define SecurityLog "/opt/log/SecurityLog"

int getGW(char *Config, char *buff, char *ConfigFile)
{
    FILE *fe = NULL;
    int fexist = 0;
	char *ptr = NULL;
    char rbuff[64] = {'\0'};//读取文件缓存

    fe = fopen(ConfigFile,"r+");
    if (NULL == fe){
            printf("no netconfig file\n");
            return -1;//未找到配置文件
    }
	
	if(0 == strlen(Config))
	{
		printf("Config is NULL\n!");
	}

    while (fgets(rbuff,sizeof(rbuff),fe)){
            if (strstr(rbuff,Config) && strchr(rbuff,':')){
                    fexist = 1;
                    break;
                    }
            else
                    memset(rbuff,'\0',sizeof(rbuff));
    }
    fclose(fe);
    if(fexist)
	{
		ptr = strchr(rbuff,'=');
		
        strcat(buff,(++ptr));
		while(ptr=strchr(buff,'\n'))
			*ptr = '\0';
	}
    else
        return -2;//未配置
    
    return 0;
}

int setGW(char *Config, char *content, char *ConfigFile)
{
	FILE *fe = NULL;
	FILE *tmp = NULL;
	char tmpfile[64] = {'\0'};
	char rbuff[64] = {'\0'};//读取文件缓存

	fe = fopen(ConfigFile,"r+");
	if (NULL == fe){
		printf("no netconfig file\n");
		return -1;
	}
	strcpy(tmpfile,ConfigFile);
	strcat(tmpfile,".bak");
	tmp = fopen(tmpfile,"w+");
	if (NULL == tmp){
		printf("tmpfile create failed\n");
		return -1;
	}

	while (fgets(rbuff,sizeof(rbuff),fe)){
		if (strstr(rbuff,Config) && (strchr(rbuff,':'))){
			continue;
		}
		else
			fwrite(rbuff,sizeof(char),strlen(rbuff),tmp);
		memset(rbuff,'\0',sizeof(rbuff));
	}

	fwrite(Config,sizeof(char),strlen(Config),tmp);
	//printf("setting 1\n");
	fwrite("=",sizeof(char),strlen("="),tmp);
	fwrite(content,sizeof(char),strlen(content),tmp);
	fwrite("\n",sizeof(char),1,tmp);
	fclose(fe);
	fclose(tmp);

	//remove(ConfigFile);
	rename(tmpfile,ConfigFile);
}


void checkSecurityEnable(Webs *wp)
{
	int ret = 0;
	char gw_ip[32] = {0};
	char gw_port[32] = {0};
	char *ptr = NULL;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	websWrite(wp,("{"));
	ret = access("/opt/tmp/startSecurity",0);
	if(-1 == ret){
		websWrite(wp,("\"start\":\"0\","));
	}else{
		websWrite(wp,("\"start\":\"1\","));
	}

	getGW("connect",gw_ip,SecurityConf);
	printf("ip = %s\n",gw_ip);
	ptr = strchr(gw_ip,':');
	if(NULL == ptr){
		websWrite(wp,("\"gw_ip\":\"0\","));
		websWrite(wp,("\"gw_port\":\"0\""));

	}else{

		*ptr = '\0';
		strcpy(gw_port,(++ptr));
		printf("ip:%s,port:%s\n",gw_ip,gw_port);

		websWrite(wp,("\"gw_ip\":\"%s\","),gw_ip);
		websWrite(wp,("\"gw_port\":\"%s\""),gw_port);
	}

	websWrite(wp,("}"));
	websDone(wp);
	return;
}

void startSecurity(Webs *wp)
{
	char *gw_ip;
	char *gw_port;
	char ipAndport[64] = {0};

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	gw_ip = websGetVar(wp,("gw_ip"),(""));
	gw_port = websGetVar(wp,("gw_port"),(""));

	strcat(ipAndport,gw_ip);
	strcat(ipAndport,":");
	strcat(ipAndport,gw_port);
	setGW("connect",ipAndport,SecurityConf);
	system("touch /opt/tmp/startSecurity");

    websWrite(wp,("安全加密功能已启动"));
	websDone(wp);
	return;
}

void cancelSecurity(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	system("rm /opt/tmp/startSecurity");

    websWrite(wp,("安全加密功能已关闭"));
	websDone(wp);
	return;
}

void getSecurityLog(Webs *wp)
{
	int ret = 0;
	char rbuff[1024] = {'\0'};
    FILE *fp = NULL;

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

    fp = fopen(SecurityLog,"r+");
    if (NULL == fp){
            printf("no %s\n",SecurityLog);
            return -1;
    }

	while(1){

		ret = fread(rbuff, 1, sizeof(rbuff)-1, fp);
		printf("read ret = %d\n",ret);
		websWrite(wp,("%s"),rbuff);
		if((sizeof(rbuff) - 1) > ret){
			printf("read end\n");
			break;
		}
		memset(rbuff,0,sizeof(rbuff));
	}
	fclose(fp);
	
	//websWrite(wp,("1\n2"));
	//paseStr(rbuff,buffer);

	websDone(wp);
	return;
}
void settingSecurity(Webs *wp)
{
	char *gw_ip;
	char *gw_port;
	char ipAndport[64] = {0};

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	gw_ip = websGetVar(wp,("gw_ip"),(""));
	gw_port = websGetVar(wp,("gw_port"),(""));

	strcat(ipAndport,gw_ip);
	strcat(ipAndport,":");
	strcat(ipAndport,gw_port);
	setGW("connect",ipAndport,SecurityConf);
	
	//system("sed -i 's/$/\r/' /opt/pingtest"); linux文件转windows


	system("touch /opt/tmp/startSecurity");


    websWrite(wp,("安全加密功能已启用"));
    websWrite(wp,("设置完成"));
	websDone(wp);
	return;
}

void querySecurity(Webs *wp)
{
	
	char gw_ip[32] = {0};
	char gw_port[32] = {0};
	char *ptr = NULL;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	getGW("connect",gw_ip,SecurityConf);
	printf("ip = %s\n",gw_ip);
	ptr = strchr(gw_ip,':');
	if(NULL == ptr){
		websWrite(wp,("{"));
		websWrite(wp,("\"gw_ip\":\"0\","));
		websWrite(wp,("\"gw_port\":\"0\""));
		websWrite(wp,("}"));

	}else{

		*ptr = '\0';
		strcpy(gw_port,(++ptr));
		printf("ip:%s,port:%s\n",gw_ip,gw_port);

		websWrite(wp,("{"));
		websWrite(wp,("\"gw_ip\":\"%s\","),gw_ip);
		websWrite(wp,("\"gw_port\":\"%s\""),gw_port);
		websWrite(wp,("}"));
	}
	websDone(wp);
	return;
}

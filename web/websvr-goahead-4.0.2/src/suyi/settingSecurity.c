#include"suyi_common.h"
#define SecurityConf "/opt/security/vpn1.0/naripcaccess.conf"
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
		//if (strstr(rbuff,Config) && (strchr(rbuff,':'))){
		if (strstr(rbuff,Config) ){
			continue;
		}
		else
			fwrite(rbuff,sizeof(char),strlen(rbuff),tmp);
		memset(rbuff,'\0',sizeof(rbuff));
	}

	fwrite(Config,sizeof(char),strlen(Config),tmp);
	//printf("setting 1\n");
	//fwrite("=",sizeof(char),strlen("="),tmp);
	fwrite(content,sizeof(char),strlen(content),tmp);
	fwrite("\n",sizeof(char),1,tmp);
	fclose(fe);
	fclose(tmp);

	//remove(ConfigFile);
	rename(tmpfile,ConfigFile);
	return 0;
}


void checkSecurityEnable(Webs *wp)
{
	int ret = 0;
	char tmp[8] = {"tmp"};
	char gw_ip[32] = {0};
	char gw_port[32] = {0};
	char mac_list[32] = {0};
	char vpn_ip[32] = {0};
	char *ptr = NULL;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	websWrite(wp,("{"));
	//ret = access("/opt/tmp/startSecurity",0);
    get_config("config","vpn1","vpn1_enable",tmp);
	ret = strcmp(tmp,"true");
	if(0 == ret){
		websWrite(wp,("\"start\":\"1\","));
		get_config("config","vpn1","gw_ip",gw_ip);
		get_config("config","vpn1","gw_port",gw_port);
		get_config("config","vpn1","mac_list",mac_list);
	}else{
    	get_config("config","vpn2","vpn2_enable",tmp);
		ret = strcmp(tmp,"true");
		if(0 == ret){
			websWrite(wp,("\"start\":\"2\","));
			get_config("config","vpn2","gw_ip",gw_ip);
			get_config("config","vpn2","gw_port",gw_port);
			get_config("config","vpn2","mac_list",mac_list);
			get_config("config","vpn2","vpn_ip",vpn_ip);
		}else{
			websWrite(wp,("\"start\":\"0\","));
			get_config("config","vpn2","gw_ip",gw_ip);
			get_config("config","vpn2","gw_port",gw_port);
			get_config("config","vpn2","mac_list",mac_list);
			get_config("config","vpn2","vpn_ip",vpn_ip);
		}
	}

	//getGW("connect",gw_ip,SecurityConf);
	printf("ip=%s,port=%s,mac_list=%s\n",gw_ip,gw_port,mac_list);

	websWrite(wp,("\"gw_ip\":\"%s\","),gw_ip);
	websWrite(wp,("\"gw_port\":\"%s\","),gw_port);
	websWrite(wp,("\"mac_list\":\"%s\","),mac_list);
	websWrite(wp,("\"vpn_ip\":\"%s\""),vpn_ip);

	websWrite(wp,("}"));
	websDone(wp);
	return;
}

void startSecurity(Webs *wp)
{
	char *vpn;
	char *gw_ip;
	char *gw_port;
	char *mac_list;
	char ipAndport[64] = {0};

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	vpn = websGetVar(wp,("vpn"),(""));
	gw_ip = websGetVar(wp,("gw_ip"),(""));
	gw_port = websGetVar(wp,("gw_port"),(""));
	mac_list = websGetVar(wp,("mac_list"),(""));

	if(strstr(vpn,"1")){
		set_config("config","vpn1","gw_ip",gw_ip,TRUE);
		set_config("config","vpn1","gw_port",gw_port,TRUE);
		set_config("config","vpn1","mac_list",mac_list,TRUE);
		strcat(ipAndport,gw_ip);
		strcat(ipAndport,":");
		strcat(ipAndport,gw_port);
		setGW("connect=",ipAndport,SecurityConf);
		system("touch /opt/tmp/startSecurity");
		set_config("config","vpn1","vpn1_enable","true",TRUE);
		websWrite(wp,("安全加密VPN1.0已启动"));
	}else if(strstr(vpn,"2")){
		set_config("config","vpn2","gw_ip",gw_ip,TRUE);
		set_config("config","vpn2","gw_port",gw_port,TRUE);
		set_config("config","vpn2","mac_list",mac_list,TRUE);
		strcat(ipAndport,gw_ip);
		strcat(ipAndport," ");
		strcat(ipAndport,gw_port);
		setGW("remote ",ipAndport,"/opt/security/vpn2.0/client.conf");
		//system("touch /opt/tmp/startSecurity");
		set_config("config","vpn2","vpn2_enable","true",TRUE);
		websWrite(wp,("安全加密VPN2.0已启动"));
	}
	websDone(wp);
	return;
}

void cancelSecurity(Webs *wp)
{
	char *vpn;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	vpn = websGetVar(wp,("vpn"),(""));

	if(strstr(vpn,"1")){
		system("rm /opt/tmp/startSecurity");
		system("rm /opt/log/SecurityLog");
		set_config("config","vpn1","vpn1_enable","false",TRUE);
		websWrite(wp,("安全加密VPN1.0已关闭"));
	}else if(strstr(vpn,"2")){
		set_config("config","vpn2","vpn2_enable","false",TRUE);
		websWrite(wp,("安全加密VPN2.0已关闭"));
	}

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

void checkdataforwardEnable(Webs *wp)
{
	int ret = 0;
	char tmp[8] = {"tmp"};
	char *ptr = NULL;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	websWrite(wp,("{"));
	//ret = access("/opt/tmp/startSecurity",0);
    get_config("config","data_forward","data_forward_enable",tmp);
	ret = strcmp(tmp,"true");
	if(0 == ret){
		websWrite(wp,("\"status\":\"1\""));
	}else{
		websWrite(wp,("\"status\":\"0\""));
	}

	websWrite(wp,("}"));
	printf("\n********%s********\n",__FUNCTION__);
	websDone(wp);
	return;
}
void data_forward(Webs *wp)
{
	char *status;

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	status = websGetVar(wp,("status"),(""));

	if(strstr(status,"1")){
		set_config("config","data_forward","data_forward_enable","true",TRUE);
		websWrite(wp,("数据转发已启动"));
	}else if(strstr(status,"0")){
		set_config("config","data_forward","data_forward_enable","false",TRUE);
		websWrite(wp,("数据转发已关闭"));
	}
	websDone(wp);
	return;
}

void set_data_forward(Webs *wp)
{
	char *data_forward_local_port;
	char *data_forward_remote_port;
	char *data_forward_remote_ip;
	char *data_forward_protocol;
	char rbuff[64] = {'\0'};
	int ret = 0;
    FILE *fp = NULL;

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	data_forward_local_port = websGetVar(wp,("data_forward_local_port"),(""));
	data_forward_protocol = websGetVar(wp,("data_forward_protocol"),(""));
	data_forward_remote_ip = websGetVar(wp,("data_forward_remote_ip"),(""));
	data_forward_remote_port = websGetVar(wp,("data_forward_remote_port"),(""));
	strcat(rbuff,data_forward_local_port);
	strcat(rbuff," ");
	strcat(rbuff,data_forward_protocol);
	strcat(rbuff," ");
	strcat(rbuff,data_forward_remote_ip);
	strcat(rbuff," ");
	strcat(rbuff,data_forward_remote_port);
	strcat(rbuff,"\n");

    fp = fopen("/opt/security/rinetd/tcpproxy.conf","a+");
    if (NULL == fp){
            printf("no /opt/security/rinetd/tcpproxy.conf\n");
			websWrite(wp,("添加失败"));
            return -1;
    }
	else{
		fwrite(rbuff,sizeof(char),strlen(rbuff),fp);
		fclose(fp);
		websWrite(wp,("添加成功"));
	}

	websDone(wp);
	return;
}

void query_data_forward(Webs *wp)
{
	int ret = 0;
	char rbuff[64] = {'\0'};
    FILE *fp = NULL;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

    fp = fopen("/opt/security/rinetd/tcpproxy.conf","r+");
    if (NULL == fp){
            printf("no /opt/security/rinetd/tcpproxy.conf\n");
            return -1;
    }

	while(1){
		ret = fread(rbuff, 1, sizeof(rbuff)-1, fp);
		//printf("read ret = %d\n",ret);
		websWrite(wp,("%s"),rbuff);
		if((sizeof(rbuff) - 1) > ret){
			printf("read end\n");
			break;
		}
		memset(rbuff,0,sizeof(rbuff));
	}
	fclose(fp);

	websDone(wp);
	return;
}

void clear_data_forward(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	system("cat /dev/null > /opt/security/rinetd/tcpproxy.conf");
	websWrite(wp,("配置文件已清空"));
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

	//getGW("connect",gw_ip,SecurityConf);
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

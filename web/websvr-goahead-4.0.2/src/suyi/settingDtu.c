#include"suyi_common.h"
#define DtuConf "/opt/config/DtuConfig"
#define DtuLog "/opt/log/DtuLog"

void queryDtu(Webs *wp)
{
	char mode[32] = {0};
	char server_ip[32] = {0};
	char server_port[32] = {0};
	char local_port[32] = {0};
	char baudrate[32] = {0};
	char data_bit[32] = {0};
	char parity[32] = {0};
	char stop_bit[32] = {0};

    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
/*	
    getConfig("mode",mode,DtuConf);
    getConfig("server_ip",server_ip,DtuConf);
    getConfig("server_port",server_port,DtuConf);
    getConfig("local_port",local_port,DtuConf);
    getConfig("baudrate",baudrate,DtuConf);
    getConfig("parity",parity,DtuConf);
    getConfig("data_bit",data_bit,DtuConf);
    getConfig("stop_bit",stop_bit,DtuConf);
	*/
    get_config("config","dtu","mode",mode);
    get_config("config","dtu","server_ip",server_ip);
    get_config("config","dtu","server_port",server_port);
    get_config("config","dtu","local_port",local_port);
    get_config("config","dtu","baudrate",baudrate);
    get_config("config","dtu","parity",parity);
    get_config("config","dtu","data_bit",data_bit);
    get_config("config","dtu","stop_bit",stop_bit);
	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

    websWrite(wp,("{"));
    websWrite(wp,("\"mode\":\"%s\","),mode);
    websWrite(wp,("\"server_ip\":\"%s\","),server_ip);
    websWrite(wp,("\"server_port\":\"%s\","),server_port);
    websWrite(wp,("\"local_port\":\"%s\","),local_port);
    websWrite(wp,("\"baudrate\":\"%s\","),baudrate);
    websWrite(wp,("\"parity\":\"%s\","),parity);
    websWrite(wp,("\"data_bit\":\"%s\","),data_bit);
    websWrite(wp,("\"stop_bit\":\"%s\""),stop_bit);
    websWrite(wp,("}"));

    websDone(wp);
	return;
}

void settingDtu(Webs *wp)
{
	char *mode;
	char *server_ip;
	char *server_port;
	char *local_port;
	char *baudrate;
	char *data_bit;
	char *parity;
	char *stop_bit;
	int ret = 0;
    printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	mode = websGetVar(wp,("mode"),("1"));
	server_ip = websGetVar(wp,("server_ip"),("0.0.0.0"));
	server_port = websGetVar(wp,("server_port"),("1010"));
	local_port = websGetVar(wp,("local_port"),("1010"));
	baudrate = websGetVar(wp,("baudrate"),("115200"));
	parity = websGetVar(wp,("parity"),("N"));
	data_bit = websGetVar(wp,("data_bit"),("8"));
	stop_bit = websGetVar(wp,("stop_bit"),("1"));

/*
	setConfig("mode",mode,DtuConf);
	setConfig("server_ip",server_ip,DtuConf);
	setConfig("server_port",server_port,DtuConf);
	setConfig("local_port",local_port,DtuConf);
	setConfig("baudrate",baudrate,DtuConf);
	setConfig("parity",parity,DtuConf);
	setConfig("data_bit",data_bit,DtuConf);
	setConfig("stop_bit",stop_bit,DtuConf);
	*/
    set_config("config","dtu","mode",mode,TRUE);
    set_config("config","dtu","server_ip",server_ip,TRUE);
    set_config("config","dtu","server_port",server_port,TRUE);
    set_config("config","dtu","local_port",local_port,TRUE);
    set_config("config","dtu","baudrate",baudrate,TRUE);
    set_config("config","dtu","parity",parity,TRUE);
    set_config("config","dtu","data_bit",data_bit,TRUE);
    set_config("config","dtu","stop_bit",stop_bit,TRUE);

	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

	ret = system("/opt/dtu/suyi_dtu &");

	printf("system ret = %d\n",ret);
	printf("system\n");
    websWrite(wp,("重启后生效"));
	websDone(wp);
	return;
}

void startDtu(Webs *wp)
{

	char *mode;
	char *server_ip;
	char *server_port;
	char *local_port;
	char *baudrate;
	char *data_bit;
	char *parity;
	char *stop_bit;
	int ret = 0;
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	//system("touch /opt/tmp/startDtu");
	set_config("config","dtu","dtu_enable","true",1);


	mode = websGetVar(wp,("mode"),("1"));
	server_ip = websGetVar(wp,("server_ip"),("0.0.0.0"));
	server_port = websGetVar(wp,("server_port"),("1010"));
	local_port = websGetVar(wp,("local_port"),("1010"));
	baudrate = websGetVar(wp,("baudrate"),("115200"));
	parity = websGetVar(wp,("parity"),("N"));
	data_bit = websGetVar(wp,("data_bit"),("8"));
	stop_bit = websGetVar(wp,("stop_bit"),("1"));

/*
	setConfig("mode",mode,DtuConf);
	setConfig("server_ip",server_ip,DtuConf);
	setConfig("server_port",server_port,DtuConf);
	setConfig("local_port",local_port,DtuConf);
	setConfig("baudrate",baudrate,DtuConf);
	setConfig("parity",parity,DtuConf);
	setConfig("data_bit",data_bit,DtuConf);
	setConfig("stop_bit",stop_bit,DtuConf);
*/
    set_config("config","dtu","mode",mode,TRUE);
    set_config("config","dtu","server_ip",server_ip,TRUE);
    set_config("config","dtu","server_port",server_port,TRUE);
    set_config("config","dtu","local_port",local_port,TRUE);
    set_config("config","dtu","baudrate",baudrate,TRUE);
    set_config("config","dtu","parity",parity,TRUE);
    set_config("config","dtu","data_bit",data_bit,TRUE);
    set_config("config","dtu","stop_bit",stop_bit,TRUE);
	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

    websWrite(wp,("串口服务已启动"));
	websDone(wp);
	return;
}

void cancelDtu(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	//system("rm /opt/tmp/startDtu");
	set_config("config","dtu","dtu_enable","false",1);

    websWrite(wp,("串口服务已关闭"));
	websDone(wp);
	return;
}


void checkDtuEnable(Webs *wp)
{
	int ret = 0;
	char enable[8] = {0};
	char mode[8] = {0};
	char server_ip[32] = {0};
	char server_port[32] = {0};
	char local_port[32] = {0};
	char baudrate[8] = {0};
	char data_bit[8] = {0};
	char parity[8] = {0};
	char stop_bit[8] = {0};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	websWrite(wp,("{"));
	//ret = access("/opt/tmp/startDtu",0);
	get_config("config","dtu","dtu_enable",enable);
	if(strcmp(enable,"true")){
		websWrite(wp,("\"start\":\"0\","));
	}else{
		websWrite(wp,("\"start\":\"1\","));
	}
	memset(mode,0,sizeof(mode));
/*
    getConfig("mode",mode,DtuConf);
    getConfig("server_ip",server_ip,DtuConf);
    getConfig("server_port",server_port,DtuConf);
    getConfig("local_port",local_port,DtuConf);
    getConfig("baudrate",baudrate,DtuConf);
    getConfig("parity",parity,DtuConf);
    getConfig("data_bit",data_bit,DtuConf);
    getConfig("stop_bit",stop_bit,DtuConf);
	*/
    get_config("config","dtu","mode",mode);
    get_config("config","dtu","server_ip",server_ip);
    get_config("config","dtu","server_port",server_port);
    get_config("config","dtu","local_port",local_port);
    get_config("config","dtu","baudrate",baudrate);
    get_config("config","dtu","parity",parity);
    get_config("config","dtu","data_bit",data_bit);
    get_config("config","dtu","stop_bit",stop_bit);
	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

    websWrite(wp,("\"mode\":\"%s\","),mode);
    websWrite(wp,("\"server_ip\":\"%s\","),server_ip);
    websWrite(wp,("\"server_port\":\"%s\","),server_port);
    websWrite(wp,("\"local_port\":\"%s\","),local_port);
    websWrite(wp,("\"baudrate\":\"%s\","),baudrate);
    websWrite(wp,("\"parity\":\"%s\","),parity);
    websWrite(wp,("\"data_bit\":\"%s\","),data_bit);
    websWrite(wp,("\"stop_bit\":\"%s\""),stop_bit);


	websWrite(wp,("}"));
	websDone(wp);
	return;

}

void getDtuLog(Webs *wp)
{
	int ret = 0;
	char rbuff[1024] = {'\0'};
    FILE *fp = NULL;

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

    fp = fopen(DtuLog,"r+");
    if (NULL == fp){
            printf("no %s\n",DtuLog);
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
	
	websDone(wp);
	return;
}

#include"suyi_common.h"
#define DtuConf "/opt/config/DtuConfig"
#define DtuLog "/opt/log/DtuLog"

void queryDtu(webs_t wp, char_t *path, char_t *query)
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
	
    getConfig("mode",mode,DtuConf);
    getConfig("server_ip",server_ip,DtuConf);
    getConfig("server_port",server_port,DtuConf);
    getConfig("local_port",local_port,DtuConf);
    getConfig("baudrate",baudrate,DtuConf);
    getConfig("parity",parity,DtuConf);
    getConfig("data_bit",data_bit,DtuConf);
    getConfig("stop_bit",stop_bit,DtuConf);
	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

    websWrite(wp,T("{"));
    websWrite(wp,T("\"mode\":\"%s\","),mode);
    websWrite(wp,T("\"server_ip\":\"%s\","),server_ip);
    websWrite(wp,T("\"server_port\":\"%s\","),server_port);
    websWrite(wp,T("\"local_port\":\"%s\","),local_port);
    websWrite(wp,T("\"baudrate\":\"%s\","),baudrate);
    websWrite(wp,T("\"parity\":\"%s\","),parity);
    websWrite(wp,T("\"data_bit\":\"%s\","),data_bit);
    websWrite(wp,T("\"stop_bit\":\"%s\""),stop_bit);
    websWrite(wp,T("}"));

    websDone(wp,200);
	return;
}

void settingDtu(webs_t wp, char_t *path, char_t *query)
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

	mode = websGetVar(wp,T("mode"),T("1"));
	server_ip = websGetVar(wp,T("server_ip"),T("0.0.0.0"));
	server_port = websGetVar(wp,T("server_port"),T("1010"));
	local_port = websGetVar(wp,T("local_port"),T("1010"));
	baudrate = websGetVar(wp,T("baudrate"),T("115200"));
	parity = websGetVar(wp,T("parity"),T("N"));
	data_bit = websGetVar(wp,T("data_bit"),T("8"));
	stop_bit = websGetVar(wp,T("stop_bit"),T("1"));


	setConfig("mode",mode,DtuConf);
	setConfig("server_ip",server_ip,DtuConf);
	setConfig("server_port",server_port,DtuConf);
	setConfig("local_port",local_port,DtuConf);
	setConfig("baudrate",baudrate,DtuConf);
	setConfig("parity",parity,DtuConf);
	setConfig("data_bit",data_bit,DtuConf);
	setConfig("stop_bit",stop_bit,DtuConf);

	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

	ret = system("/opt/dtu/suyi_dtu &");

	printf("system ret = %d\n",ret);
	printf("system\n");
    websWrite(wp,T("重启后生效"));
	websDone(wp,200);
	return;
}

void startDtu(webs_t wp, char_t *path, char_t *query)
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
	system("touch /opt/config/startDtu");

	mode = websGetVar(wp,T("mode"),T("1"));
	server_ip = websGetVar(wp,T("server_ip"),T("0.0.0.0"));
	server_port = websGetVar(wp,T("server_port"),T("1010"));
	local_port = websGetVar(wp,T("local_port"),T("1010"));
	baudrate = websGetVar(wp,T("baudrate"),T("115200"));
	parity = websGetVar(wp,T("parity"),T("N"));
	data_bit = websGetVar(wp,T("data_bit"),T("8"));
	stop_bit = websGetVar(wp,T("stop_bit"),T("1"));


	setConfig("mode",mode,DtuConf);
	setConfig("server_ip",server_ip,DtuConf);
	setConfig("server_port",server_port,DtuConf);
	setConfig("local_port",local_port,DtuConf);
	setConfig("baudrate",baudrate,DtuConf);
	setConfig("parity",parity,DtuConf);
	setConfig("data_bit",data_bit,DtuConf);
	setConfig("stop_bit",stop_bit,DtuConf);

	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

    websWrite(wp,T("串口服务已启动"));
	websDone(wp,200);
	return;
}

void cancelDtu(webs_t wp, char_t *path, char_t *query)
{
	printf("\n********%s********\n",__FUNCTION__);

	system("rm /opt/config/startDtu");

    websWrite(wp,T("串口服务已关闭"));
	websDone(wp,200);
	return;
}


void checkDtuEnable(webs_t wp, char_t *path, char_t *query)
{
	int ret = 0;
	char mode[32] = {0};
	char server_ip[32] = {0};
	char server_port[32] = {0};
	char local_port[32] = {0};
	char baudrate[32] = {0};
	char data_bit[32] = {0};
	char parity[32] = {0};
	char stop_bit[32] = {0};
	printf("\n********%s********\n",__FUNCTION__);

	websWrite(wp,T("{"));
	ret = access("/opt/config/startDtu",0);
	if(-1 == ret){
		websWrite(wp,T("\"start\":\"0\","));
	}else{
		websWrite(wp,T("\"start\":\"1\","));
	}

    getConfig("mode",mode,DtuConf);
    getConfig("server_ip",server_ip,DtuConf);
    getConfig("server_port",server_port,DtuConf);
    getConfig("local_port",local_port,DtuConf);
    getConfig("baudrate",baudrate,DtuConf);
    getConfig("parity",parity,DtuConf);
    getConfig("data_bit",data_bit,DtuConf);
    getConfig("stop_bit",stop_bit,DtuConf);
	printf("mode:%s,server_ip:%s,server_port:%s,local_port:%s,baudrate:%s,parity:%s,data_bit:%s,stop_bit:%s\n",mode,server_ip,server_port,local_port,baudrate,parity,data_bit,stop_bit);

    websWrite(wp,T("\"mode\":\"%s\","),mode);
    websWrite(wp,T("\"server_ip\":\"%s\","),server_ip);
    websWrite(wp,T("\"server_port\":\"%s\","),server_port);
    websWrite(wp,T("\"local_port\":\"%s\","),local_port);
    websWrite(wp,T("\"baudrate\":\"%s\","),baudrate);
    websWrite(wp,T("\"parity\":\"%s\","),parity);
    websWrite(wp,T("\"data_bit\":\"%s\","),data_bit);
    websWrite(wp,T("\"stop_bit\":\"%s\""),stop_bit);


	websWrite(wp,T("}"));
	websDone(wp,200);
	return;

}

void getDtuLog(webs_t wp, char_t *path, char_t *query)
{
	int ret = 0;
	char rbuff[1024] = {'\0'};
    FILE *fp = NULL;

	printf("\n********%s********\n",__FUNCTION__);

    fp = fopen(DtuLog,"r+");
    if (NULL == fp){
            printf("no %s\n",DtuLog);
            return -1;
    }

	while(1){

		ret = fread(rbuff, 1, sizeof(rbuff)-1, fp);
		printf("read ret = %d\n",ret);
		websWrite(wp,T("%s"),rbuff);
		if((sizeof(rbuff) - 1) > ret){
			printf("read end\n");
			break;
		}
		memset(rbuff,0,sizeof(rbuff));
	}
	fclose(fp);
	
	websDone(wp,200);
	return;
}

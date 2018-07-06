#include"suyi_common.h"
#define PingLog "/opt/log/PingLog"

void paseStr(char *rbuff, char *buffer)
{
	char *otr = NULL;
	char *ptr = NULL;
	char *qtr = NULL;
	printf("rbuff = %s\n",rbuff);
	qtr = rbuff;
	while(ptr = strchr(qtr,'\n')){
		otr = ++ptr;
		*ptr = '\0';
		strcat(buffer,qtr);
		strcat(buffer,"\r\n");
		qtr = otr;
	}
	printf("buffer = %s\n",buffer);

}

void cleanLog(webs_t wp, char_t *path, char_t *query)
{
	system("echo > /opt/log/PingLog");
	websDone(wp,200);
	return;
}
void getPingLog(webs_t wp, char_t *path, char_t *query)
{
	int ret = 0;
	char rbuff[1024] = {'\0'};
    FILE *fp = NULL;

	printf("\n********%s********\n",__FUNCTION__);

	//system("sed -i 's/$/\r/' /opt/security/log"); linux文件转windows
    fp = fopen(PingLog,"r+");
    if (NULL == fp){
            printf("no %s\n",PingLog);
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
	
	//websWrite(wp,T("1\n2"));
	//paseStr(rbuff,buffer);

	websDone(wp,200);
	return;
}

void pingTest(webs_t wp, char_t *path, char_t *query)
{
	char *pingAddr;
	char *pingCount;
	char *ping[128] = {'\0'};
	char *rbuff[1024] = {'\0'};
	char *buffer[1024] = {'\0'};

	printf("\n********%s********\n",__FUNCTION__);
	system("echo > /opt/log/PingLog");

	pingAddr = websGetVar(wp,T("pingAddr"),T(""));
	pingCount = websGetVar(wp,T("pingCount"),T(""));

	strcat(ping,"ping -c ");
	strcat(ping,pingCount);
	strcat(ping," ");
	strcat(ping,pingAddr);
	strcat(ping," >> /opt/log/PingLog &");
	printf("ping:%s,%d\n",ping,atoi(pingCount));
	system(ping);
	//system("sed -i 's/$/\r/' /opt/pingtest"); linux文件转windows
/*
    fp = fopen("/opt/pingtest","r+");
    if (NULL == fp){
            printf("no pingtest file\n");
            return -1;
    }
	fread(rbuff, 1, 1023, fp);
	printf("%s\n", rbuff);
	*/

	//paseStr(rbuff,buffer);

    websWrite(wp,T("测试完成"));
	websDone(wp,200);
	return;
}

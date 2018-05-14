#include"suyi_common.h"

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
void pingTest(webs_t wp, char_t *path, char_t *query)
{
	char *pingAddr;
	char *pingCount;
	char *ping[64] = {'\0'};
	char *rbuff[1024] = {'\0'};
	char *buffer[1024] = {'\0'};
    FILE *fp = NULL;

	printf("********pingTest********\n");
	pingAddr = websGetVar(wp,T("pingAddr"),T(""));
	pingCount = websGetVar(wp,T("pingCount"),T(""));

	strcat(ping,"ping -c ");
	strcat(ping,pingCount);
	strcat(ping," ");
	strcat(ping,pingAddr);
	strcat(ping," > /opt/pingtest");
	printf("ping:%s\n",ping);
	system(ping);
	//system("sed -i 's/$/\r/' /opt/pingtest"); linux文件转windows

	sleep(8);
    fp = fopen("/opt/pingtest","r+");
    if (NULL == fp){
            printf("no pingtest file\n");
            return -1;
    }
	fread(rbuff, 1, 1023, fp);
	printf("%s\n", rbuff);
	fclose(fp);

	//paseStr(rbuff,buffer);

    websWrite(wp,T("%s"),rbuff);
	websDone(wp,200);
	return;
}

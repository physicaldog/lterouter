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

void start_sshd(Webs *wp)
{
	char *arg = NULL;
	char *ptr = NULL;
    FILE *fp = NULL;
	char rbuff[1024] = {'\0'};
	char line[16] = {'\0'};

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	
	arg = websGetVar(wp,("arg"),("stop"));
	if(!strcmp(arg,"start")){
		//fp = popen(" ps | grep dropbear |wc -l","r");
		fp = popen(" ps | grep dropbear | grep -v grep | wc -l","r");
		if(fp){
			fread(line,1,sizeof(line),fp);
			fclose(fp);
			printf("%s\n",line);
			if('1' == line[0]){
				websWrite(wp,("已开启！"));
			}
			else{
				fp = popen("/etc/init.d/S50dropbear start" ,"r");
				if(fp){
					fread(rbuff,1,sizeof(rbuff),fp);
					if(strstr(rbuff,"OK"))
						websWrite(wp,("远程调试已开启！"));
					else
						websWrite(wp,("远程调试打开失败！"));
					fclose(fp);
				}
			}
		}
	}
	else if(!strcmp(arg,"stop")){
		//fp = popen(" ps | grep dropbear |wc -l","r");
		fp = popen(" ps | grep dropbear | grep -v grep | wc -l","r");
		if(fp){
			fread(line,1,sizeof(line),fp);
			fclose(fp);
			printf("%s\n",line);
			if('0' == line[0])
				websWrite(wp,("已关闭！"));
			else{
				fp = popen("/etc/init.d/S50dropbear stop" ,"r");
				if(fp){
					fread(rbuff,1,sizeof(rbuff),fp);
					if(strstr(rbuff,"OK"))
						websWrite(wp,("远程调试已关闭！"));
					else
						websWrite(wp,("远程调试关闭失败！"));
					fclose(fp);
				}
			}
		}
	}

	websDone(wp);
	return;
}


void cleanLog(Webs *wp)
{
	system("echo > /opt/log/PingLog");
	websDone(wp);
	return;
}
void getPingLog(Webs *wp)
{
	int ret = 0;
	char rbuff[1024] = {'\0'};
    FILE *fp = NULL;

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	//system("sed -i 's/$/\r/' /opt/security/log"); linux文件转windows
    fp = fopen(PingLog,"r+");
    if (NULL == fp){
            printf("no %s\n",PingLog);
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

void pingTest(Webs *wp)
{
	char *pingAddr;
	char *pingCount;
	char *ping[128] = {'\0'};
	char *rbuff[1024] = {'\0'};
	char *buffer[1024] = {'\0'};

	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	system("echo > /opt/log/PingLog");

	pingAddr = websGetVar(wp,("pingAddr"),(""));
	pingCount = websGetVar(wp,("pingCount"),(""));

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

    websWrite(wp,("测试完成"));
	websDone(wp);
	return;
}

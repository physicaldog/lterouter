#include"suyi_common.h"
#define ApnConf "/opt/config/ApnConfig"

int imsi_read(int fd,char *IMSI)
{/*{{{*/
	char rbuff[4096] = {'\0'};
	char* ptr = rbuff;
	int ret=0;
	while (1) {
		ret = read(fd, ptr, 0x01);
		if (1 == ret) {
			if ('\n' != *ptr) {
				ptr++;
			}
			else {
				printf("%s",rbuff);
				if(strlen(rbuff) < 15)
					return -1;
				else
					strcpy(IMSI,rbuff);
				return 0;
			}
		}
		else {
			syslog(LOG_DEBUG,"at_read_err:\n");
			return -1;
		}
	}
	syslog(LOG_DEBUG,"read exit\n");
}/*}}}*/

void queryIMSI(webs_t wp, char_t *path, char_t *query)
{
	int fd = 0, i = 0,ret = 0;
	char IMSI[64] = {'\0'};
	
	for(i=0;i<3;i++){
		fd = openDev(SerPort);
		if(fd > 0)
			break;
		else
			printf("IMIS openDev Failed!\n");
	}

	tcflush(fd,TCIFLUSH);
	at_send(fd,"at+cimi\r\n");
	for(i=0;i<3;i++){
		ret = imsi_read(fd,IMSI);
		if(ret >= 0)
			break;
		else
			printf("IMIS read Failed!\n");
	}
	printf("IMSI:%s",IMSI);
    websWrite(wp,T("%s"),IMSI);
	websDone(wp,200);
	return;
}

void settingAPN(webs_t wp, char_t *path, char_t *query)
{
	char *apn;

	apn = websGetVar(wp,T("apn"),T("jk"));
	printf("settingAPN apn=%s\n",apn);

	setConfig("apn:",apn,ApnConf);

    websWrite(wp,T("重启后生效"));
	websDone(wp,200);
	return;
}

void queryAPN(webs_t wp, char_t *path, char_t *query)
{
        char buff[64] = {'\0'};//读取文件缓存

        printf("apn query\n");
        getConfig("apn:",buff,ApnConf);
        printf("apn = %s\n", buff);

        websWrite(wp,T("%s"),buff);

        websDone(wp,200);
        return;
}

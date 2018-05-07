/*************************************************************************
	> File Name: a6g2cimpl.c
	> Author: huaqiyan
	> Mail: huaqiyan@zlg.cn
	> Created Time: Tue 01 Jul 2014 08:42:21 AM CST
 ************************************************************************/
/*
 * 	Vendor program for the GoAhead WebServer. This is a demonstration
 *	program to control the device with web server.
 */

#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif

#include "a6g2cimpl.h"
#include "router_status.h"
#define ConfigFile "/opt/config/netconfig"

//void deviceInfo(webs_t wp, char_t *path, char_t *query)
//{
//		//char buff[] = {""}
//        //websWrite(wp,T("%s"),buff);
//		printf("deviceinfo:");
//        websWrite(wp,T("monkey"));
//
//        websDone(wp,200);
//        return;
//}
//
//void CPUUsageRate(webs_t wp, char_t *path, char_t *query)
//{
//
//	char *name;
//	printf("CPUUsageRate:");
//	name = websGetVar(wp,T("name"),T("jk"));
//	printf("%s\n",name);
//	websDone(wp,200);
//	return;
//}

int getConfig(char *Config, char *buff)
{
    FILE *fe = NULL;
    int fexist = 0;
    char rbuff[64] = {'\0'};//读取文件缓存

    fe = fopen(ConfigFile,"r+");
    if (NULL == fe){
            printf("no netconfig file\n");
            return -1;//未找到配置文件
    }

    while (fgets(rbuff,sizeof(rbuff),fe)){
            if (strstr(rbuff,Config)){
                    fexist = 1;
                    break;
                    }
            else
                    memset(rbuff,'\0',sizeof(rbuff));
    }
    fclose(fe);
    if(fexist)
        strcat(buff,rbuff+strlen(Config));
    else
        return -2;//未配置
    
    return 0;
}

void lanip_query(webs_t wp, char_t *path, char_t *query)
{
    char buff[64] = {'\0'};

    printf("lanip query\n");
    getConfig("lanip:",buff);
    printf("lan = %s\n",buff);

    websWrite(wp,T("%s"),buff);

    websDone(wp,200);
	return;
}

void apn_query(webs_t wp, char_t *path, char_t *query)
{
        FILE *fe = NULL;
        char buff[64] = {'\0'};//读取文件缓存

        printf("apn query\n");
        getConfig("apn:",buff);
        printf("apn = %s\n", buff);

        websWrite(wp,T("%s"),buff);

        websDone(wp,200);
        return;
}

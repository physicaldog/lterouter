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
#include "router_set.h"
#include "router_status.h"

#define ConfigFile "/opt/config/netconfig"
#define tmpFile "/opt/config/netconfig.bak"

int setlanip(char *ifname,char *ip, char *netmask)
{
    char buff[64] = {'\0'};
    getConfig("lanip",buff);
    set_ip("eth1",buff,"255.255.255.0");

    return 0;

}


int setConfig(char *Config, char *content)
{
    FILE *fe = NULL;
    FILE *tmp = NULL;
    char rbuff[64] = {'\0'};//读取文件缓存

    fe = fopen(ConfigFile,"r+");
    if (NULL == fe){
            printf("no netconfig file\n");
            return -1;
    }

    tmp = fopen(tmpFile,"w+");
    if (NULL == tmp){
            printf("tmpfile create failed\n");
            return -1;
    }

    while (fgets(rbuff,sizeof(rbuff),fe)){
            if (strstr(rbuff,Config)){
                    continue;
            }
            else
                    fwrite(rbuff,sizeof(char),strlen(rbuff),tmp);
                    memset(rbuff,'\0',sizeof(rbuff));
    }
    fwrite(Config,sizeof(char),strlen(Config),tmp);
    fwrite(content,sizeof(char),strlen(content),tmp);
    fwrite("\n",sizeof(char),1,tmp);
    fclose(fe);
    fclose(tmp);

    remove(ConfigFile);
    rename(tmpFile,ConfigFile);
    
}

void lanip_set(webs_t wp, char_t *path, char_t *query)
{
	char *lanip;
    lanip = websGetVar(wp,T("lanip"),T("jk"));
    printf("lanip=%s\n",lanip);

    setConfig("lanip:",lanip);

    set_ip("eth1",lanip,"255.255.255.0");

    websDone(wp,200);
	return;
}

void apn_set(webs_t wp, char_t *path, char_t *query)
{
	char *apn;

    apn = websGetVar(wp,T("apn"),T("jk"));
    printf("apn=%s\n",apn);

    setConfig("apn:",apn);

    websDone(wp,200);
	return;
}

void sysReset(webs_t wp, char_t *path, char_t *query)
{
    printf("sysReset\n");
    websDone(wp,200);
	return;
}

void moduleReset(webs_t wp, char_t *path, char_t *query)
{
    printf("moduleReset\n");
    websDone(wp,200);
	return;
}


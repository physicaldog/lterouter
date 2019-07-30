#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "uci.h"

#define LanConf "/opt/config/LanConfig"
#define ApnConf "/opt/config/ApnConfig"
#define LAN "eth0"
#define LANIP "192.168.1.1"
#define WAN "usb0"
int get_config(char *package,char *section,char *option,char *buff) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};

	sprintf(path,"%s.%s.%s",package,section,option); 

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	if((UCI_OK == uci_lookup_ptr(c, &p, path, true)) && (p.flags & UCI_LOOKUP_COMPLETE)) { 

		strcpy(buff,p.o->v.string);
	} 
	else{
		log_msg("%s not found! ",path);
		//uci_perror(c, "\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}

int set_config(char *package,char *section,char *option,char *value,int commit) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};
	int ret = 0;

	sprintf(path,"%s.%s.%s",package,section,option); 

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

//	printf("%s\n",path);
	if((UCI_OK == uci_lookup_ptr(c, &p, path, true)) ) { 
		p.value = value;
		ret = uci_set(c,&p);
		uci_save(c,p.p);
		if(commit){
			uci_commit(c,&p.p,false);
		}
	} 
	else{
		log_msg("%s not found!\n",path);
		//uci_perror(c, "\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}
int getConfig(char *Config, char *buff, char *ConfigFile)
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
            if (strstr(rbuff,Config)){
                    fexist = 1;
                    break;
                    }
            else
                    memset(rbuff,'\0',sizeof(rbuff));
    }
    fclose(fe);
    if(fexist)
	{
		ptr = strchr(rbuff,':');
		
        strcat(buff,(++ptr));
		while(ptr=strchr(buff,'\n'))
			*ptr = '\0';
	}
    else
        return -2;//未配置
    
    return 0;
}

int setConfig(char *Config, char *content, char *ConfigFile)
{
	FILE *fe = NULL;
	FILE *tmp = NULL;
	char tmpfile[64] = {'\0'};
	char rbuff[64] = {'\0'};//读取文件缓存

	printf("setConfig : %s\n",Config);
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
		if (strstr(rbuff,Config)){
			continue;
		}
		else
			fwrite(rbuff,sizeof(char),strlen(rbuff),tmp);
		memset(rbuff,'\0',sizeof(rbuff));
	}

	fwrite(Config,sizeof(char),strlen(Config),tmp);
	//printf("setting 1\n");
	fwrite(":",sizeof(char),strlen(":"),tmp);
	fwrite(content,sizeof(char),strlen(content),tmp);
	fwrite("\n",sizeof(char),1,tmp);
	fclose(fe);
	fclose(tmp);

	//remove(ConfigFile);
	rename(tmpfile,ConfigFile);
}
#if 0
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
		strcat(buff,rbuff+strlen(Config)+1);
	else
		return -2;//未配置

	return 0;
}

int setConfig(char *Config, char *content)
{
	FILE *fe = NULL;
	FILE *tmp = NULL;
	char rbuff[64] = {'\0'};//读取文件缓存

	fe = fopen("/opt/config/udhcpd.conf","r+");
	if (NULL == fe){
		printf("no netconfig file\n");
		return -1;
	}

	tmp = fopen("/opt/config/udhcpd.conf.bak","w+");
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
	fwrite(" ",sizeof(char),1,tmp);
	fwrite(content,sizeof(char),strlen(content),tmp);
	fwrite("\n",sizeof(char),1,tmp);
	fclose(fe);
	fclose(tmp);

	remove("/opt/config/udhcpd.conf");
	rename("/opt/config/udhcpd.conf.bak","/opt/config/udhcpd.conf");
}
#endif
int get_local_ip(char * ifname, char * ip)
{
	char *temp = NULL;
	int inet_sock;
	struct ifreq ifr;

	inet_sock = socket(AF_INET, SOCK_DGRAM, 0); 

	memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
	memcpy(ifr.ifr_name, ifname, strlen(ifname));

	if(0 != ioctl(inet_sock, SIOCGIFADDR, &ifr)) 
	{   
		perror("ioctl error");
		return -1;
	}

	temp = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);     
	memcpy(ip, temp, strlen(temp));

	close(inet_sock);

	return 0;
}

int set_ip(char * ifname, char * ip, char * netmask)
{
	char *temp = NULL;
	int inet_sock;
	int ret;
	struct ifreq ifr;

	inet_sock = socket(AF_INET, SOCK_DGRAM, 0); 

	memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
	memcpy(ifr.ifr_name, ifname, strlen(ifname));

	if(0 != ioctl(inet_sock, SIOCGIFNETMASK, &ifr)) 
	{   
		perror("ioctl error");
		return -1;
	}

	//设置ip netmask
	ret = inet_aton(ip,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
	if(ret == 0){
		log_msg("SET IP FAILED!\n");
		return -1;
	}
	if(0 != ioctl(inet_sock, SIOCSIFADDR, &ifr)) 
	{   
		log_msg("ioctl error ip");
		close(inet_sock);
		return -1;
	}

	ret = inet_aton(netmask,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
	if(ret == 0)
		log_msg("SET NETMASK FAILED!\n");
	if(0 != ioctl(inet_sock, SIOCSIFNETMASK, &ifr)) 
	{   
		log_msg("ioctl error netmask");
		close(inet_sock);
		return -1;
	}

	close(inet_sock);

	return 0;
}


void setcontent(FILE *fd,char *Config, char *content)
{
	fwrite(Config,sizeof(char),strlen(Config),fd);
	fwrite(" ",sizeof(char),1,fd);
	fwrite(content,sizeof(char),strlen(content),fd);
	fwrite("\n",sizeof(char),1,fd);
	
}
int setDhcpConf(char *ip)
{
	FILE *fd = NULL;
	char *p = NULL;
	char ip_str[32] = {'\0'};
	int ret = 0;

	fd = fopen("/opt/config/udhcpd.conf","w+");
	if (NULL == fd){
		printf("no udhcpd.conf file\n");
		return -1;
	}
	strcpy(ip_str,ip);
	p = strrchr(ip_str,'.');
	ret = atoi(++p);
	sprintf(p,"%d",++ret);
	setcontent(fd,"start",ip_str);
	p = strrchr(ip_str,'.');
	strcpy(++p,"254\n");
	setcontent(fd,"end",ip_str);
	setcontent(fd,"option subnet","255.255.255.0");
	setcontent(fd,"opt router",ip);
	setcontent(fd,"interface","eth0");
	setcontent(fd,"max_leases","24");

	fclose(fd);

}

int lanInit()
{
	int ret = 0;
	char* p = NULL;
	char lanip[32] = {'\0'};
	char netmask[32] = {'\0'};
	char lanip_end[32] = {'\0'};
	char ip[32] = {'\0'};

	//ret = getConfig("lanip",lanip,LanConf);
	ret = get_config("config","lan","netmask",netmask);
	ret = get_config("config","lan","ip",lanip);
	//printf("%s:%s\n",lanip,netmask);
	
	if(0 == ret){
		ret = set_ip(LAN,lanip,netmask);
		if(0 != ret){
			set_ip(LAN,LANIP,"255.255.255.0");
		}
	}
	else{
		set_ip(LAN,LANIP,"255.255.255.0");
	}

	get_local_ip("eth0",ip);
	setDhcpConf(ip);
	system("udhcpd /opt/config/udhcpd.conf");

	return 0;

}

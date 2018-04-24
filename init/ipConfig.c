#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define ConfigFile "/opt/config/netconfig"
#define LAN "eth0"
#define LANIP "192.168.1.1"
#define WAN "usb0"

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
		printf("SET IP FAILED!\n");
		return -1;
	}
	if(0 != ioctl(inet_sock, SIOCSIFADDR, &ifr)) 
	{   
		perror("ioctl error ip");
		return -1;
	}

	ret = inet_aton(netmask,&((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
	if(ret == 0)
		printf("SET NETMASK FAILED!\n");
	if(0 != ioctl(inet_sock, SIOCSIFNETMASK, &ifr)) 
	{   
		perror("ioctl error netmask");
		return -1;
	}

	close(inet_sock);

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
	char ip_end[32] = {'\0'};

	fd = fopen("/opt/config/udhcpd.conf","w+");
	if (NULL == fd){
		printf("no udhcpd.conf file\n");
		return -1;
	}
	p = strrchr(ip,'.');
	p[1]++;
	//p[1] = itoa(atoi(p[1])+1);
	setcontent(fd,"start",ip);
	strcpy(ip_end,ip);
	p = strrchr(ip_end,'.');
	strcpy(++p,"254\n");
	setcontent(fd,"end",ip_end);
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
	char lanip_end[32] = {'\0'};
	char ip[32] = {'\0'};

	ret = getConfig("lanip",lanip);
	
	if(0 == ret){
		ret = set_ip(LAN,lanip,"255.255.255.0");
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

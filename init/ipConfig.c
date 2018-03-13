#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define ConfigFile "/opt/config/netconfig"
#define LAN "eth0"
#define LANIP "192.168.99.1"
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

int lanInit()
{
	int ret = 0;
	char lanip[32] = {'\0'};

	ret = getConfig("lanip",lanip);
	if(0 == ret){
		ret = set_ip(LAN,lanip,"255.255.255.0");
		if(0 != ret)
			set_ip(LAN,LANIP,"255.255.255.0");
	}
	else{
		set_ip(LAN,LANIP,"255.255.255.0");
		
	}
	return 0;

}

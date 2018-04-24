#include"suyi_common.h"
extern char netmode;
extern char netStatus;
extern char simStatus;
extern char ecmStatus;
extern char csq[];
extern int rssi;


int get_local_ip(char *ifname, char *ip, char *netmask, char *macaddr)
{
	char *temp = NULL;
	int inet_sock;
	int ret = 0;
	struct ifreq ifr;
	struct sockaddr_in *sin = NULL;

	inet_sock = socket(AF_INET, SOCK_DGRAM, 0); 

	memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
	memcpy(ifr.ifr_name, ifname, strlen(ifname));

	if(0 != ioctl(inet_sock, SIOCGIFADDR, &ifr)) 
	{   
		printf("ioctl ip error\n");
		return -1;
	}

	sin = (struct sockaddr_in *)&(ifr.ifr_addr);
	strcpy(ip,inet_ntoa(sin->sin_addr));
//	temp = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);     
//	memcpy(ip, temp, strlen(temp));

	if(0 != ioctl(inet_sock, SIOCGIFNETMASK, &ifr)) 
	{   
		printf("ioctl macaddr error\n");
		return -1;
	}
	sin = (struct sockaddr_in *)&(ifr.ifr_netmask);
	strcpy(netmask,inet_ntoa(sin->sin_addr));
	printf("ret=%s,%d\n",netmask,strlen(inet_ntoa(sin->sin_addr)));

	if(0 != ioctl(inet_sock, SIOCGIFHWADDR, &ifr)) 
	{   
		printf("ioctl netmask error\n");
		return -1;
	}
	sprintf(macaddr,"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
					(unsigned char)ifr.ifr_hwaddr.sa_data[0],
					(unsigned char)ifr.ifr_hwaddr.sa_data[1],
					(unsigned char)ifr.ifr_hwaddr.sa_data[2],
					(unsigned char)ifr.ifr_hwaddr.sa_data[3],
					(unsigned char)ifr.ifr_hwaddr.sa_data[4],
					(unsigned char)ifr.ifr_hwaddr.sa_data[5]
					);

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

int getConfig(char *Config, char *buff)
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
        strcat(buff,rbuff+strlen(Config));
		while(ptr=strchr(buff,'\n'))
			*ptr = '\0';
	}
    else
        return -2;//未配置
    
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
#if 1
int get_csq(char *buff)
{
	int i=0;
	char *ptr = NULL;
	char *qtr = NULL;
	ptr = strchr(buff,' ');
	qtr = strchr(buff,',');
	printf("csq=%s\n",buff);
	for(i=0;i<4;i++)
	{
		if(ptr == qtr)
			break;
		ptr++;
		if(ptr == qtr)
			break;
		printf("ptr=%c ",*ptr);
		csq[i] = *ptr;
	}
	rssi = atoi(csq);
	printf("csq=%s,rssi=%d\n",csq,rssi);
	return 0;
}
int get_sysinfoex(char *buff)
{
	int i=0;
	char *ptr = NULL;
	char *qtr = NULL;
	ptr = strchr(buff,',');
	netStatus = *(--ptr);
	for(i;i<5;i++)
	{
		ptr = strchr(ptr,',');
		++ptr;
		if(2 == i)
			simStatus = *ptr;
		if(4 == i)
			netmode = *ptr;
	}
	return 0;
}

int get_ndisstat(char *buff)
{
	int i=0;
	char *ptr = NULL;
	ptr = strchr(buff,',');
	ecmStatus = *(--ptr);

	return 0;
}

int direct_process(char *buff)
{
	char *ptr = NULL;
	char *at_arr[16] = {
		"SRVST",
		"NDISSTAT",
		"SYSINFOEX",
		"NDISSTATQRY",
		"CSQ",
	};

	if(strstr(buff,"ERROR"))
		return 0;
	if (strstr(buff,"CSQ")) {
		get_csq(buff);
		return 0;
	}
	if ('^' == buff[0]) {
		//syslog(LOG_DEBUG,"Get direct:%s\n",buff);
		//ptr = strstr(buff,at_arr[0]);
		if ((strstr(buff,at_arr[0])) || (strstr(buff,at_arr[2]))) {
			get_sysinfoex(buff);
			return 0;
		}
		//ptr = strstr(buff,at_arr[1]);
		if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
			get_ndisstat(buff);
			return 0;
		}
	}
}

int at_read(int fd)
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
				//printf("%s",rbuff);
				//syslog(LOG_DEBUG,"Get direct: %s\n",rbuff);
				ret = direct_process(rbuff);
				if(0 == ret)
					return 0;
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
			}
		}
		else {
			syslog(LOG_DEBUG,"at_read_err:\n");
			return -1;
		}
	}
	syslog(LOG_DEBUG,"read exit\n");
}/*}}}*/

int at_send(int fd,char* at)
{/*{{{*/
	int ret;
	ret = write(fd, at, strlen(at));
	if(ret < 0){
		syslog(LOG_DEBUG,"at_send_error!\n");
		return -1;
	}
	return 0 ;

}/*}}}*/
#endif
int openDev(char *Dev)
{/*{{{*/
	int iFd;

	struct termios opt; 

	//iFd = open(Dev, O_RDWR | O_NOCTTY | O_NONBLOCK);                        
	iFd = open(Dev, O_RDWR | O_NOCTTY);                        
	if(iFd < 0) {
		syslog(LOG_DEBUG,"open %s failed!\n",Dev);
		return -1;
	}

	tcgetattr(iFd, &opt);      

	//cfsetispeed(&opt, B57600);
	//cfsetospeed(&opt, B57600);

	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);


	opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG | ECHOE);
	opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
	opt.c_oflag   &=   ~(OPOST);
	opt.c_cflag   &=   ~(CSIZE   |   PARENB);
	opt.c_cflag   |=   CS8;

	opt.c_cc[VMIN]   =   DATA_LEN;                                      
	opt.c_cc[VTIME]  =   150;

	if (tcsetattr(iFd,   TCSANOW,   &opt)<0) {
		return   -1;
	}


	return iFd;
}/*}}}*/

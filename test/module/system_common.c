#include "system_common.h"

#define DATA_LEN	0xFF  /* test data's len */
struct timeval get_uptime(void)
{
	struct timeval now;
	unsigned char  timestr[60] = {0};
	unsigned char  uptimestr[30] = {0};
	unsigned char * dotaddr;
	unsigned long second;
	char error = 0;
	FILE * timefile = NULL;

	timefile = fopen("/proc/uptime", "r");
	if(!timefile)
	{
		printf("[%s:line:%d] error opening '/proc/uptime'",__FILE__,__LINE__);
		error = 1;
		goto out;
	}

	if( (fread(timestr, sizeof(char), 60, timefile)) == 0 )
	{
		printf("[%s:line:%d] read '/proc/uptime' error",__FILE__,__LINE__);
		error = 1;
		goto out;
	}

	dotaddr = strchr(timestr, '.');
	if((dotaddr - timestr + 2) < 30)
		memcpy(uptimestr, timestr, dotaddr - timestr + 2);
	else
	{
		printf("[%s:line:%d] uptime string is too long",__FILE__,__LINE__);
		error = 1;
		goto out;
	}
	uptimestr[dotaddr - timestr + 2] = '\0';

out:
	if(error)
	{
		now.tv_sec  = 0;
		now.tv_usec = 0;
	}
	else
	{
		now.tv_sec  = atol(uptimestr);
		now.tv_usec = 0;
	}

	fclose(timefile);
	return now;
}

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
		printf("%s not found! ",path);
		uci_perror(c, "\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}

int set_config(char *package,char *section,char *option,char *value,bool commit) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};
	int ret = 0;

	sprintf(path,"%s.%s.%s",package,section,option); 

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	printf("%s\n",path);
	if((UCI_OK == uci_lookup_ptr(c, &p, path, true)) && (p.flags & UCI_LOOKUP_COMPLETE)) { 
		p.value = value;
		ret = uci_set(c,&p);
		uci_save(c,p.p);
		if(commit){
			uci_commit(c,&p.p,false);
		}
	} 
	else{
		printf("%s not found!\n",path);
		uci_perror(c, "\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}

int openDev(char *Dev)
{/*{{{*/
	int iFd;
	struct termios opt; 
	printf("\n****%s****\n",__FUNCTION__);
	iFd = open(Dev, O_RDWR | O_NOCTTY);                        
	if(iFd < 0) {
		printf("open %s failed!\n",Dev);
		return -1;
	}
	tcgetattr(iFd, &opt);      
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

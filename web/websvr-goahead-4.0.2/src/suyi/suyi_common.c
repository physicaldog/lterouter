#include "suyi_common.h"
#include "./uci.h" 
extern char netmode;
extern char netStatus;
extern char simStatus;
extern char ecmStatus;
extern char csq[];
extern int rssi;
extern int  rsrp;
extern int  sinr;
extern int  rsrq;
extern char  imei[];
int setTime_done = 0;


#define Version "/opt/config/Version"
#define MD5 "/opt/md5.sum"
void testFun(Webs *wp)
{
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	//websWrite(wp, "Name %s", websGetVar(wp, "name", "")); 
	//websWrite(wp,  "Age %s", websGetVar(wp, "age", ""));
	websWrite(wp, "Name monkey\n"); 
	//websWrite(wp,  "Age 20");
	websDone(wp);

}

#if 1
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

int set_config(char *package,char *section,char *option,char *value,int commit) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};
	int ret = 0;

	sprintf(path,"%s.%s.%s",package,section,option); 

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	//printf("%s\n",path);
	if((UCI_OK == uci_lookup_ptr(c, &p, path, true))) { 
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

#if 1
int get_hcsq(char *buff)
{
	int i=0;
	char sysmode[8] = {0};
	char rssib[32] = {0};
	char rsrpb[32] = {0};
	char sinrb[32] = {0};
	char rsrqb[32] = {0};
	char *ptr = NULL;
	char *qtr = NULL;
	printf("********get_hcsq=%s********\n",buff);
	ptr = strstr(buff,"LTE");
	if(NULL == ptr)
		return 0;
	ptr = strstr(buff,"\n");
	*ptr = '\0';
	ptr = strchr(buff,',');
	strcpy(rssib,(++ptr));

	ptr = strchr(rssib,',');
	strcpy(rsrpb,(++ptr));

	ptr = strchr(rsrpb,',');
	strcpy(sinrb,(++ptr));

	ptr = strchr(sinrb,',');
	strcpy(rsrqb,(++ptr));
	
	qtr = strchr(rssib,',');
	*qtr = '\0';

	qtr = strchr(rsrpb,',');
	*qtr = '\0';
	
	qtr = strchr(sinrb,',');
	*qtr = '\0';
	
	rssi = atoi(rssib);
	rsrp = atoi(rsrpb);
	sinr = atoi(sinrb);
	rsrq = atoi(rsrqb);
	printf("rssi=%s rsrp=%s sinr=%s rsrq=%s\n",rssib,rsrpb,sinrb,rsrqb);
	printf("rssi=%d rsrp=%d sinr=%d rsrq=%d\n",rssi,rsrp,sinr,rsrq);
	return 0;
}

int get_cgsn(char *buff)
{
	int i=0;
	char *ptr = NULL;
	char *qtr = NULL;
	printf("\n********%s********\n",__FUNCTION__);
	printf("cgsn buff:%s\n",buff);
	ptr = strchr(buff,'\"');
	if(ptr){
		printf("imei:%s\n",imei);
		sprintf(imei,"%s",ptr+1);
		ptr = NULL;
		ptr = strchr(imei,'\"');
		if(ptr)
			*ptr = '\0';
	}else
		printf("no imei\n");
		
	printf("imei:%s\n",imei);
	return 0;
}

int get_csq(char *buff)
{
	int i=0;
	char *ptr = NULL;
	char *qtr = NULL;
	printf("\n********%s********\n",__FUNCTION__);
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
	printf("\n********%s********\n",__FUNCTION__);
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
	printf("\n********%s********\n",__FUNCTION__);
	ptr = strchr(buff,',');
	ecmStatus = *(--ptr);

	return 0;
}

void get_nwtime(char *buff)
{
	int i = 0,ret = 0;
	char *ptr = NULL;
	char *qtr = NULL;
	char time[32] = {'\0'};
	char hour[4] = {'\0'};

	printf("%s\n",__FUNCTION__);
	printf("buff:%s\n",buff);
	ptr = strstr(buff," ");
	if(NULL == ptr)
		return;
	ptr++;
	if('9' == *ptr){
		setTime_done = 0;
		return;
	}
	else{
		sprintf(time,"date -s \"20");
		for(i=0;i<2;i++){
			qtr = strchr(ptr,'/');
			if(qtr == NULL){
				printf("qtr failed\n");
				return;
			}
			printf("%c\n",*qtr);
			*qtr = '-';
		}
//		qtr = strchr(ptr,',')
//		memcpy()
		qtr = strchr(ptr,',');
		*qtr = ' ';
		qtr = strchr(ptr,'+');
		memset(qtr,'\0',strlen(qtr));
		strcat(time,ptr);
		strcat(time,"\"");
		printf("%s\n",time);
		system(time);
		setTime_done = 1;
	}

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
		"HCSQ",
	};
	printf("\n********%s********\n%s\n",__FUNCTION__,buff);
	if((strlen(buff)<=3) || strstr(buff,"AT^"))
		return -1;

	if(strstr(buff,"ERROR") || strstr(buff,"OK") )
		return 0;
	
	if (strstr(buff,"+CGSN:")) {
		get_cgsn(buff);
		return 0;
	}
	if (strstr(buff,"HCSQ")) {
		get_hcsq(buff);
		return 0;
	}
	if ('^' == buff[0]) {
		syslog(LOG_DEBUG,"Get direct:%s\n",buff);
		//ptr = strstr(buff,at_arr[0]);
		if ((strstr(buff,at_arr[2]))) {
			get_sysinfoex(buff);
			return 0;
		}
		//ptr = strstr(buff,at_arr[1]);
		if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
			get_ndisstat(buff);
			return 0;
		}
		if(strstr(buff,"NWTIME")){
			//get_nwtime(buff);
			return 0;
		}
	}
}

int at_read(int fd)
{/*{{{*/
	char rbuff[4096] = {'\0'};
	char* ptr = rbuff;
	int ret=0;
	printf("\n********%s********\n",__FUNCTION__);
	while (1) {
		ret = read(fd, ptr, 0x01);
		if (1 == ret) {
			if ('\n' != *ptr) {
				ptr++;
			}
			else {
				//printf("%s",rbuff);
				//syslog(LOG_DEBUG,"Get direct: %s\n",rbuff);
				//printf("111111111ret = %d\n",ret);
				ret = direct_process(rbuff);
				if(0 == ret)
					return 0;
				//printf("ret = %d\n",ret);
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
			}
		}
		else {
			printf("at_read_err:\n");
			return -1;
		}
	}
	syslog(LOG_DEBUG,"read exit\n");
}/*}}}*/

int at_send(int fd,char* at)
{/*{{{*/
	int ret;
	printf("\n********%s********,%s\n",__FUNCTION__,at);
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
#endif

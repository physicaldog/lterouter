#include "suyi_dtu.h"

#if 1
int UdpClient_Mode()
{
	int ret = 0;
	int fd = 0;
	int fd_max = 0;
	int sockfd = 0;
	int addr_len = 0;
	char ip_addr[32] = {0};

	struct sockaddr_in host_addr;
	struct sockaddr_in client_addr;

	char server_ip[32] = {0};
	char server_port[8] = {0};
	char local_port[8] = {0};
	char baudrate[8] = {0};
	char parity[8] = {0};
	char data_bit[8] = {0};
	char stop_bit[8] = {0};

	fd_set rdfds;
	struct timeval timeout;
	
	char rbuff[1024] = {0};
	char tbuff[1024] = {0};

	log_msg("\n********%s*********\n",__FUNCTION__);

	/*初始化select 读集合*/
	FD_ZERO(&rdfds);
	get_SerialConf(baudrate,parity,data_bit,stop_bit);
	/*初始化串口*/
	fd = openSer(SerPort,baudrate,parity,data_bit,stop_bit);
	if(0 >= fd){
		log_ret("openSer failed!");
		/*串口打开失败则关闭sock*/
		return -1;
	}
	/*清空缓冲区*/
	tcflush(fd, TCIOFLUSH);
	if(fd > fd_max)
		fd_max = fd;

	getConfig("server_ip",server_ip,DtuConf);
	if(0 == strlen(server_ip)){
		log_msg("server_ip not config!\n");
		return -1;
	}
	getConfig("server_port",server_port,DtuConf);
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	inet_pton(AF_INET,server_ip,&(host_addr.sin_addr.s_addr));
	if(atoi(server_port))
		host_addr.sin_port = htons(atoi(server_port));
	else{
		log_msg("server_port not config\n");
		return -1;
	}

	getConfig("local_port",local_port,DtuConf);
	bzero(&client_addr,sizeof(struct sockaddr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(atoi(local_port));
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	//sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd){
		log_ret("socket create failed!");
		return -1;
	}
	if(sockfd > fd_max)
		fd_max = sockfd;

	ret = bind(sockfd,(struct sockaddr *)&client_addr,sizeof(struct sockaddr));
	if(-1 == ret){
		log_ret("bind failed");
		return -1;
	}
	addr_len =  sizeof(struct sockaddr);
	while(1){
		log_msg("start select\n");
		//timeout.tv_sec = 10;
		FD_SET(fd,&rdfds);
		FD_SET(sockfd,&rdfds);

		ret = select(fd_max+1,&rdfds,NULL,NULL,&timeout);
		if(-1 == ret){
			log_ret("select");
			return -1;
		}else{
			if(FD_ISSET(sockfd,&rdfds)){
				//log_msg("read socket message\n");
				//ret = read(sockfd,rbuff,sizeof(rbuff));
				ret = recvfrom(sockfd,rbuff,sizeof(rbuff),0,(struct sockaddr *)&client_addr,&addr_len);
				if(0 > ret){
					log_ret("recvfrom failed");
					//清理缓冲区
					FD_CLR(sockfd,&rdfds);
					//break;
				}else{
					log_msg("MESSAGE from socket:%s\n",rbuff);
					ret = write(fd,rbuff,strlen(rbuff));
					//fsync(fd);
					tcdrain(fd);
					memset(rbuff,0,sizeof(rbuff));
					//FD_SET(sockfd,&rdfds);
				}
			}
			if(FD_ISSET(fd,&rdfds)){
				//log_msg("read serial message\n");
				ret = read(fd,tbuff,sizeof(tbuff));
				if(0 == ret){
					log_ret("ret = 0");
				}else if(0 > ret){
					log_ret("read serial failed");
					//清理缓冲区
				}else{
					log_msg("MESSAGE from serial:%s\n",tbuff);
					//错误处理
					//ret=write(sockfd,tbuff,strlen(tbuff));
					ret = sendto(sockfd,tbuff,strlen(tbuff),0,(struct sockaddr *)&host_addr,sizeof(struct sockaddr));
					memset(tbuff,0,sizeof(tbuff));
					//FD_SET(sockfd,&rdfds);
				}
			}
		}
	}
/*
	while(1){
		log_msg("start: %s,%d\n",inet_ntop(AF_INET,&(host_addr.sin_addr.s_addr),ip_addr,sizeof(ip_addr)),ntohs(host_addr.sin_port));
		ret = sendto(sockfd,"TEST",strlen("TEST"),0,(struct sockaddr *)&host_addr,sizeof(struct sockaddr));
		log_msg("ret=%d\n",ret);
		sleep(2);	
	}
*/
}
#else
int main()
{
	int ret = 0;
	int sockfd = 0;
	int addr_len = 0;
	char server_port[8] = {0};
	char buffer[256] = {"monkey"};
	char ip_addr[32] = {0};

	struct sockaddr_in host_addr;
	
	log_msg("%s\n",__FUNCTION__);

	//getConfig("server_port",server_port,DtuConf);
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	if(atoi(server_port))
		host_addr.sin_port = htons(atoi(server_port));
	else
		host_addr.sin_port = htons(DEF_PORT);

	inet_pton(AF_INET,"192.168.1.1",(void *)&(host_addr.sin_addr.s_addr));

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	//sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd){
		log_ret("socket create failed!");
		return -1;
	}

	while(1){
		ret = sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&host_addr,sizeof(host_addr));
		sleep(1);
	}
}
#endif

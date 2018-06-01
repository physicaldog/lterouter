#include "suyi_dtu.h"

int main()
{
	int ret = 0;
	int fd = 0;
	int sockfd = 0;
	int nsockfd = 0;
	int fd_max = 0;
	int addr_len = 0;
	int host_ip = 0;
	int client_port = 0;
	int keepIdle = 3; // 如该连接在3秒内没有任何数据往来,则进行探测 
	int keepInterval = 1; // 探测时发包的时间间隔为1秒
	int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
	char client_ip[32] = {0};
	struct sockaddr_in host_addr;
	struct sockaddr_in client_addr;
	struct tcp_info tcpInfo;

	fd_set rdfds;
	struct timeval timeout;

	char rbuff[1024] = {0};
	char tbuff[1024] = {0};

	struct termios termptr;

	printf("********%s********\n",__FUNCTION__);

	/*初始化tcpServer*/
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT);
	ret = inet_pton(AF_INET,HOST_IP,&(host_addr.sin_addr.s_addr));
	if(-1 == ret){
		perror("sockfd open failed");
		return -1;
	}
	else if(0 == ret){
		printf("无效地址,采用本地任意地址\n");
		host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	sockfd = tcpSev_init(&host_addr,CLIENT_NUM);
	if(-1 == sockfd){
		perror("sockfd open failed");
		return -1;
	}

	/*初始化select 读集合*/
	FD_ZERO(&rdfds);
	FD_SET(fd,&rdfds);
	//FD_SET(sockfd,&rdfds);

	while(1){
		printf("start accept:\n");
		addr_len = sizeof(struct sockaddr_in);
		bzero(&client_addr,addr_len);
		nsockfd = accept(sockfd,(struct sockaddr *)&client_addr,&addr_len);
		if(-1 == nsockfd){
			perror("accpet failed");
			return -1;
		}
		client_port = ntohs(client_addr.sin_port);
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip));	
		printf("client form:%s:%d\n",client_ip,client_port);

		set_tcp_keepAlive(nsockfd, keepIdle, keepInterval,keepCount);

		FD_SET(nsockfd,&rdfds);
		if(nsockfd > fd_max)
			fd_max = nsockfd;

		/*初始化串口*/
		fd = openSer(SerPort);
		if(0 >= fd){
			perror("openSer failed!");
			/*串口打开失败则关闭sock*/
			close(nsockfd);
			return -1;
		}
		/*清空缓冲区*/
		tcflush(fd, TCIOFLUSH);

		FD_SET(fd,&rdfds);
		if(fd > fd_max)
			fd_max = fd;

		while(1){
			printf("start select\n");
			timeout.tv_sec = 10;
			FD_SET(fd,&rdfds);
			FD_SET(nsockfd,&rdfds);
			ret = select(fd_max+1,&rdfds,NULL,NULL,&timeout);
			if(-1 == ret){
				perror("select");
				return -1;
			}else if(0 == ret){
				printf("Timeout!\n");
				
				/*
				addr_len = sizeof(tcpInfo);
				getsockopt(nsockfd,IPPROTO_TCP,TCP_INFO,&tcpInfo,(socklen_t*)&addr_len);
				if((tcpInfo.tcpi_state == TCP_ESTABLISHED)){
					printf("please wait!\n");
				}else{
					printf("nsockfd closed!\n");
						FD_CLR(nsockfd,&rdfds);
						close(nsockfd);
						break;
				}
				*/
				ret = recv(nsockfd,rbuff,sizeof(rbuff),MSG_DONTWAIT);
				printf("ret =%d\n",ret);
				if((0 > ret)){
					if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)){
						printf("please wait!\n");
					}else{
						printf("网络断开\n");
						FD_CLR(nsockfd,&rdfds);
						close(fd);
						close(nsockfd);
						break;
					}
					}else if(0 == ret){
						printf("链接关闭!\n");
						FD_CLR(nsockfd,&rdfds);
						close(fd);
						close(nsockfd);
						break;
					}
				}else{
				//用select检测accept有问题
				/*
				   if(FD_ISSET(sockfd,&rdfds)){

				   printf("start accept:\n");
				   addr_len = sizeof(struct sockaddr_in);
				   bzero(&client_addr,addr_len);
				   nsockfd = accept(sockfd,(struct sockaddr *)&client_addr,&addr_len);
				   if(-1 == nsockfd){
				   perror("accpet failed");
				   return -1;
				   }
				   client_port = ntohs(client_addr.sin_port);
				   inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip));	
				   printf("client form:%s:%d\n",client_ip,client_port);

				   FD_SET(nsockfd,&rdfds);
				   if(nsockfd > fd_max)
				   fd_max = nsockfd;

				   }*/
				if(FD_ISSET(nsockfd,&rdfds)){
					//printf("read socket message\n");
					ret = read(nsockfd,rbuff,sizeof(rbuff));
					if(0 == ret){
						perror("链接关闭！");
						FD_CLR(nsockfd,&rdfds);
						close(fd);
						close(nsockfd);
						break;
					}else if(0 > ret){
						perror("read failed");
						//清理缓冲区
						FD_CLR(nsockfd,&rdfds);
						break;
					}else{
						printf("MESSAGE from socket:%s\n",rbuff);
						ret = write(fd,rbuff,strlen(rbuff));
						//fsync(fd);
						tcdrain(fd);
						memset(rbuff,0,sizeof(rbuff));
						//FD_SET(nsockfd,&rdfds);
					}
				}
				if(FD_ISSET(fd,&rdfds)){
					//printf("read serial message\n");
					ret = read(fd,tbuff,sizeof(tbuff));
					if(0 == ret){
						perror("ret = 0");
					}else if(0 > ret){
						perror("read serial failed");
						//清理缓冲区
					}else{
						printf("MESSAGE from serial:%s\n",rbuff);
						/*错误处理*/
						ret=write(nsockfd,tbuff,strlen(tbuff));
						memset(tbuff,0,sizeof(tbuff));
						//FD_SET(nsockfd,&rdfds);
					}
				}
			}
		}
	}

	close(fd);
	close(sockfd);
	close(nsockfd);
	return 0;
}

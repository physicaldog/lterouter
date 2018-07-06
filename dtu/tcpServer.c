/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
//#include <linux/tcp.h>

#define HOST_IP ""
#define PORT 1010
#define CLIENT_NUM 2
 */
#include "suyi_dtu.h"

int tcpSev_init(struct sockaddr_in *host_addr,int client_num)
{
	int ret = 0;
	int sockfd = 0;
	printf("********%s********\n",__FUNCTION__);

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd){
		perror("sockfd open failed");
		return -1;
	}

	ret = bind(sockfd,(struct sockaddr *)host_addr,sizeof(struct sockaddr_in));
	if(-1 == ret){
		perror("bind failed");
		return -1;
	}

	ret = listen(sockfd,client_num);
	if(-1 == ret){
		perror("listen failed");
		return -1;
	}

	return sockfd;

}

int set_tcp_keepAlive(int fd, int start, int interval, int count)  
{  
	int keepAlive = 1; // 开启keepalive属性

	if (fd < 0 || start < 0 || interval < 0 || count < 0) return -1;  //入口参数检查 ，编程的好习惯。
	//启用心跳机制，如果您想关闭，将keepAlive置零即可  
	if(setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1)  
	{  
		perror("setsockopt");  
		return -1;  
	}  
	//启用心跳机制开始到首次心跳侦测包发送之间的空闲时间  
	if(setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,(void *)&start,sizeof(start)) == -1)  
	{  
		perror("setsockopt");  
		return -1;  
	}  
	//两次心跳侦测包之间的间隔时间  
	if(setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,(void *)&interval,sizeof(interval)) == -1)  
	{  
		perror("setsockopt");  
		return -1;  
	}  
	//探测次数，即将几次探测失败判定为TCP断开  
	if(setsockopt(fd,SOL_TCP,TCP_KEEPCNT,(void *)&count,sizeof(count)) == -1)  
	{  
		perror("setsockopt");  
		return -1;  
	}  
	return 0;  
}
#if 1
int TcpServer_Mode()
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

	char local_port[8] = {0};
	char baudrate[8] = {0};
	char parity[8] = {0};
	char data_bit[8] = {0};
	char stop_bit[8] = {0};

	fd_set rdfds;
	struct timeval timeout;

	char rbuff[1024] = {0};
	char tbuff[1024] = {0};

	struct termios termptr;

	printf("********%s********\n",__FUNCTION__);

	getConfig("local_port",local_port,DtuConf);
	get_SerialConf(baudrate,parity,data_bit,stop_bit);

	/*初始化tcpServer*/
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(atoi(local_port));
	//监听所有地址
	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sockfd = tcpSev_init(&host_addr,CLIENT_NUM);
	if(-1 == sockfd){
		perror("sockfd open failed");
		return -1;
	}


	while(1){
		printf("start accept:\n");
		addr_len = sizeof(struct sockaddr_in);
		bzero(&client_addr,addr_len);
		nsockfd = accept(sockfd,(struct sockaddr *)&client_addr,&addr_len);
		if(-1 == nsockfd){
			perror("accpet failed");
			return -1;
		}
		if(nsockfd > fd_max)
			fd_max = nsockfd;

		client_port = ntohs(client_addr.sin_port);
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip));	
		printf("client form:%s:%d\n",client_ip,client_port);

		set_tcp_keepAlive(nsockfd, keepIdle, keepInterval,keepCount);


		/*初始化串口*/
		fd = openSer(SerPort,baudrate,parity,data_bit,stop_bit);
		if(0 >= fd){
			perror("openSer failed!");
			/*串口打开失败则关闭sock*/
			close(nsockfd);
			return -1;
		}
		if(fd > fd_max)
			fd_max = fd;
		/*清空缓冲区*/
		tcflush(fd, TCIOFLUSH);


		while(1){
			printf("start select\n");
			/*初始化select 读集合*/
			FD_ZERO(&rdfds);
			FD_SET(fd,&rdfds);
			FD_SET(nsockfd,&rdfds);
			timeout.tv_sec = 10;

			ret = select(fd_max+1,&rdfds,NULL,NULL,&timeout);
			if(-1 == ret){
				perror("select");
				return -1;
			}else if(0 == ret){
				printf("Timeout!\n");

				ret = recv(nsockfd,rbuff,sizeof(rbuff),MSG_DONTWAIT);
				printf("ret =%d\n",ret);
				if((0 > ret)){
					if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)){
						printf("please wait!\n");
					}else{
						printf("网络断开\n");
						FD_CLR(nsockfd,&rdfds);
						FD_CLR(fd,&rdfds);
						close(fd);
						close(nsockfd);
						break;
					}
				}else if(0 == ret){
					printf("链接关闭!\n");
					FD_CLR(nsockfd,&rdfds);
					FD_CLR(fd,&rdfds);
					close(fd);
					close(nsockfd);
					break;
				}
			}else{
				if(FD_ISSET(nsockfd,&rdfds)){
					ret = read(nsockfd,rbuff,sizeof(rbuff));
					if(0 == ret){
						perror("read 链接关闭！");
						FD_CLR(nsockfd,&rdfds);
						FD_CLR(fd,&rdfds);
						close(fd);
						close(nsockfd);
						break;
					}else if(0 > ret){
						perror("socket read failed");
						//清理缓冲区
						FD_CLR(nsockfd,&rdfds);
						FD_CLR(fd,&rdfds);
						close(fd);
						close(nsockfd);
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
						printf("MESSAGE from serial:%s\n",tbuff);
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
	close(nsockfd);
	close(sockfd);
	return 0;
}
#else 
int main()
{
	int ret = 0;
	int sockfd = 0;
	int nsockfd = 0;
	int fd_max = 0;
	int addr_len = 0;
	int host_ip = 0;
	int client_port = 0;
	char client_ip[32] = {0};
	char rbuff[1024] = {0};
	char wbuff[1024] = {0};
	struct sockaddr_in host_addr;
	struct sockaddr_in client_addr;
	struct tcp_info tcpInfo;

	fd_set rdfds;
	struct timeval timeout;

	int keepIdle = 3; // 如该连接在60秒内没有任何数据往来,则进行探测 
	int keepInterval = 1; // 探测时发包的时间间隔为5 秒
	int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.

	printf("********%s********\n",__FUNCTION__);

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

	//fd_max = sockfd;
	FD_ZERO(&rdfds);
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

		while(1){
			printf("start select\n");
			timeout.tv_sec = 10;
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
						close(nsockfd);
						break;
					}
				}else if(0 == ret){
					printf("链接关闭!\n");
					FD_CLR(nsockfd,&rdfds);
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
					printf("read message\n");
					ret = read(nsockfd,rbuff,sizeof(rbuff));
					if(0 == ret){
						perror("链接关闭！");
						FD_CLR(nsockfd,&rdfds);
						close(nsockfd);
						break;
					}else if(0 > ret){
						perror("read failed");
						//清理缓冲区
						FD_CLR(nsockfd,&rdfds);
						break;
					}else{
						printf("read:%s\n",rbuff);
						memset(rbuff,0,sizeof(rbuff));
						//FD_SET(nsockfd,&rdfds);
					}
				}
			}
		}
	}
	return 0;
}
#endif

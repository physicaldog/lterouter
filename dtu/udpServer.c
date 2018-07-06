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
*/

#include "suyi_dtu.h"
#define HOST_IP ""
#define PORT 1010
#define CLIENT_NUM 2

#if 1
int UdpServer_Mode()
{
	int ret = 0;
	int fd = 0;
	int fd_max = 0;
	int sockfd = 0;
	int addr_len = 0;
	char ip_addr[32] = {0};

	struct sockaddr_in host_addr;
	struct sockaddr_in client_addr;

	char local_port[8] = {0};
	char baudrate[8] = {0};
	char parity[8] = {0};
	char data_bit[8] = {0};
	char stop_bit[8] = {0};

	fd_set rdfds;
	struct timeval timeout;
	
	char rbuff[1024] = {0};
	char tbuff[1024] = {0};

	printf("\n********%s*********\n",__FUNCTION__);

	/*初始化select 读集合*/
	FD_ZERO(&rdfds);
	get_SerialConf(baudrate,parity,data_bit,stop_bit);
	/*初始化串口*/
	fd = openSer(SerPort,baudrate,parity,data_bit,stop_bit);
	if(0 >= fd){
		perror("openSer failed!");
		/*串口打开失败则关闭sock*/
		return -1;
	}
	/*清空缓冲区*/
	tcflush(fd, TCIOFLUSH);
	if(fd > fd_max)
		fd_max = fd;

	getConfig("local_port",local_port,DtuConf);
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	if(atoi(local_port))
		host_addr.sin_port = htons(atoi(local_port));
	else
		host_addr.sin_port = htons(DEF_PORT);

	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == sockfd){
		perror("socket create failed!");
		return -1;
	}
	if(sockfd > fd_max)
		fd_max = sockfd;

	ret = bind(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
	if(-1 == ret){
		perror("bind failed");
		return -1;
	}
	addr_len =  sizeof(struct sockaddr);

#if 1
	while(1){
		printf("start select\n");
		//timeout.tv_sec = 10;
		FD_SET(fd,&rdfds);
		FD_SET(sockfd,&rdfds);

		ret = select(fd_max+1,&rdfds,NULL,NULL,&timeout);
		if(-1 == ret){
			perror("select");
			return -1;
		}else{
			if(FD_ISSET(sockfd,&rdfds)){
				//printf("read socket message\n");
				//ret = read(sockfd,rbuff,sizeof(rbuff));
				ret = recvfrom(sockfd,rbuff,sizeof(rbuff),0,(struct sockaddr *)&client_addr,&addr_len);
				if(0 > ret){
					perror("recvfrom failed");
					//清理缓冲区
					FD_CLR(sockfd,&rdfds);
					//break;
				}else{
					printf("MESSAGE from socket:%s\n",rbuff);
					ret = write(fd,rbuff,strlen(rbuff));
					//fsync(fd);
					tcdrain(fd);
					memset(rbuff,0,sizeof(rbuff));
					//FD_SET(sockfd,&rdfds);
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
					//错误处理
					//ret=write(sockfd,tbuff,strlen(tbuff));
					ret = sendto(sockfd,tbuff,strlen(tbuff),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
					memset(tbuff,0,sizeof(tbuff));
					//FD_SET(sockfd,&rdfds);
				}
			}
		}
	}
#else
	
	while(1){
		memset(rbuff,0,sizeof(rbuff));
		ret = recvfrom(sockfd,rbuff,sizeof(rbuff),0,(struct sockaddr *)&client_addr,&addr_len);
		printf("recvfrom:%s,%s,%d\n",inet_ntop(AF_INET,(void *)&(client_addr.sin_addr.s_addr),ip_addr,sizeof(ip_addr)),rbuff,ntohs(client_addr.sin_port));
		ret = sendto(sockfd,rbuff,strlen(rbuff),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
		printf("ret = %d\n",ret);
	}
#endif
	close(fd);
	close(sockfd);

}
#else
int main()
{
	int ret = 0;
	int sockfd = 0;
	int addr_len = 0;
	char local_port[8] = {0};
	char rbuff[256] = {0};
	char ip_addr[32] = {0};

	struct sockaddr_in host_addr;
	struct sockaddr_in client_addr;
	
	printf("%s\n",__FUNCTION__);

	//getConfig("local_port",local_port,DtuConf);
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	if(atoi(local_port))
		host_addr.sin_port = htons(10000);
	else
		host_addr.sin_port = htons(DEF_PORT);

	host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bzero(&client_addr,sizeof(struct sockaddr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(1010);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = inet_pton(AF_INET,"192.168.1.1",&(client_addr.sin_addr.s_addr));

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == sockfd){
		perror("socket create failed!");
		return -1;
	}

	ret = bind(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
	if(-1 == ret){
		perror("bind failed");
		return -1;
	}

	while(1){
		sleep(2);
		memset(rbuff,0,sizeof(rbuff));
		ret = sendto(sockfd,"TEST",strlen("TEST"),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
		//ret = recvfrom(sockfd,rbuff,sizeof(rbuff),0,(struct sockaddr *)&client_addr,&addr_len);
		//printf("recvfrom:%s,%s\n",inet_ntop(AF_INET,(void *)&(client_addr.sin_addr.s_addr),ip_addr,sizeof(ip_addr)),rbuff);
	}
}
#endif

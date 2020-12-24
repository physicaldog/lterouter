/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define HOST_IP "192.168.1.1"
#define HOST_PORT 1010
*/
#include "suyi_dtu.h"
#if 1
int set_tcpkeepAlive(int fd, int start, int interval, int count)  
{  
	int keepAlive = 1; // 开启keepalive属性

	if (fd < 0 || start < 0 || interval < 0 || count < 0) return -1;  //入口参数检查 ，编程的好习惯。
	//启用心跳机制，如果您想关闭，将keepAlive置零即可  
	if(setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1)  
	{  
		log_ret("setsockopt");  
		return -1;  
	}  
	//启用心跳机制开始到首次心跳侦测包发送之间的空闲时间  
	if(setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,(void *)&start,sizeof(start)) == -1)  
	{  
		log_ret("setsockopt");  
		return -1;  
	}  
	//两次心跳侦测包之间的间隔时间  
	if(setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,(void *)&interval,sizeof(interval)) == -1)  
	{  
		log_ret("setsockopt");  
		return -1;  
	}  
	//探测次数，即将几次探测失败判定为TCP断开  
	if(setsockopt(fd,SOL_TCP,TCP_KEEPCNT,(void *)&count,sizeof(count)) == -1)  
	{  
		log_ret("setsockopt");  
		return -1;  
	}  
	return 0;  
}
int TcpClient_Mode()
{
	int ret = 0;
	int fd = 0;
	int fd_max = 0;
	int sockfd = 0;
	struct sockaddr_in host_addr;

	char server_ip[32] = {0};
	char server_port[8] = {0};

	char baudrate[8] = {0};
	char parity[8] = {0};
	char data_bit[8] = {0};
	char stop_bit[8] = {0};

	fd_set rdfds;
	struct timeval timeout;

	char rbuff[1024] = {0};
	char tbuff[1024] = {0};

	int keepIdle = 3; // 如该连接在3秒内没有任何数据往来,则进行探测 
	int keepInterval = 1; // 探测时发包的时间间隔为1秒
	int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
	log_msg("\n********%s*********\n",__FUNCTION__);

	//getConfig("server_ip",server_ip,DtuConf);
	get_config("config","dtu","server_ip",server_ip);
	if(0 == strlen(server_ip)){
		log_msg("server_ip not config!\n");
		return -1;
	}
	//getConfig("server_port",server_port,DtuConf);
	get_config("config","dtu","server_port",server_port);
	if(atoi(server_port))
		host_addr.sin_port = htons(atoi(server_port));
	else{
		log_msg("server_port not config\n");
		return -1;
	}
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



	/*初始化网络*/
	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(atoi(server_port));

	ret = inet_pton(AF_INET,server_ip,&(host_addr.sin_addr.s_addr));
	log_msg("%s,%s\n",server_ip,server_port);


	while(1){
		log_msg("new socket!\n");
		sockfd = socket(AF_INET,SOCK_STREAM,0);
		if(-1 == sockfd){
			log_ret("socket open failed!");
			return -1;
		}
		if(sockfd > fd_max)
			fd_max = sockfd;

		set_tcpkeepAlive(sockfd, keepIdle, keepInterval,keepCount);

		ret = connect(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
		if(-1 == ret){
			log_ret("connet failed!");
			close(sockfd);
			sleep(2);
			continue;
		}

		log_msg("connet host!\n");
		while(1){
			/*初始化select 读集合*/
			FD_ZERO(&rdfds);
			timeout.tv_sec = 10;
			FD_SET(fd,&rdfds);
			FD_SET(sockfd,&rdfds);

			ret = select(fd_max+1,&rdfds,NULL,NULL,&timeout);
			if(-1 == ret){
				log_ret("select");
				return -1;
			}else if(0 == ret){
				log_msg("Timeout!\n");

				ret = recv(sockfd,rbuff,sizeof(rbuff),MSG_DONTWAIT);
				log_msg("ret =%d\n",ret);
				if((0 > ret)){
					if((errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)){
						log_msg("please wait!\n");
					}else{
						log_msg("网络断开\n");
						break;
					}
				}else if(0 == ret){
					log_msg("链接关闭!\n");
					break;
				}
			}else{
				if(FD_ISSET(sockfd,&rdfds)){
					ret = read(sockfd,rbuff,sizeof(rbuff));
					if(0 == ret){
						log_ret("read 链接关闭！");
						break;
					}else if(0 > ret){
						log_ret("read failed");
						break;
					}else{
						log_msg("MESSAGE from socket:%s\n",rbuff);
						ret = write(fd,rbuff,strlen(rbuff));
						tcdrain(fd);
						memset(rbuff,0,sizeof(rbuff));
					}
				}
				if(FD_ISSET(fd,&rdfds)){
					ret = read(fd,tbuff,sizeof(tbuff));
					if(0 == ret){
						log_ret("ret = 0");
						break;
					}else if(0 > ret){
						log_ret("read serial failed");
						break;
					}else{
						log_msg("MESSAGE from serial:%s\n",tbuff);
						ret=write(sockfd,tbuff,strlen(tbuff));
						memset(tbuff,0,sizeof(tbuff));
					}
				}
			}
		}
	}
	close(sockfd);
	close(fd);
}
#else
int main()
{
	int ret = 0;
	int sockfd = 0;

	struct sockaddr_in host_addr;

	bzero(&host_addr,sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(HOST_PORT);

	ret = inet_pton(AF_INET,HOST_IP,&(host_addr.sin_addr.s_addr));

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == sockfd){
		log_ret("socket open failed!");
		return -1;
	}

	ret = connect(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
	if(-1 == ret){
		log_ret("connet failed!");
		return -1;
	}
	log_msg("connect host!\n");


	while(1){
		ret = write(sockfd,"monkey",6);
		usleep(500000);
		ret = write(sockfd,"123456",6);
		log_msg("ret= %d\n",ret);
		sleep(5);
	}
	close(sockfd);
}
#endif

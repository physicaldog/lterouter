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
		perror("socket open failed!");
		return -1;
	}

	ret = connect(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
	if(-1 == ret){
		perror("connet failed!");
		return -1;
	}
	printf("connect host!\n");
	
	
	while(1){
		ret = write(sockfd,"monkey",6);
		usleep(500000);
		ret = write(sockfd,"123456",6);
		printf("ret= %d\n",ret);
		sleep(5);
	}
	close(sockfd);
}

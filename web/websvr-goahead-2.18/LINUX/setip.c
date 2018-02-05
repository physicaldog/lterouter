#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IF_NAME "eth0"

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
/*
int main(void)
{
        char ip[32] = {"192.168.2.10"};
        char netmask[32] = {"255.255.0.0"};

        set_ip("eth1",ip,netmask);
        //set_ip("eth1","192.168.2.88","255.255.0.0");


        return 0;
}
*/

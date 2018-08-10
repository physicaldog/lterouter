#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <syslog.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define DtuConf "/opt/config/DtuConfig"
/*serial*/
#define SerPort "/dev/ttymxc2"
#define BAUDRATE 9600
#define DATA_BIT 8
#define PARITY 'N'
#define STOP_BIT 1
#define Vmin 255
#define Vtime 1

void setTermios(struct termios *termptr,char *baudrate,char *parity,char *data_bit,char *stop_bit);
void serRead(int fd, char *buffer, unsigned int len);
void serWrite(int fd, char *buffer, unsigned int len);
int openSer(char *SerDev,char *baudrate,char *parity,char *data_bit,char *stop_bit);

/*tcpServer*/
#define DEF_PORT 1010
#define LOACL_IP ""
#define CLIENT_NUM 2
int tcpSev_init(struct sockaddr_in *host_addr,int client_num);
int set_tcp_keepAlive(int fd, int start, int interval, int count);

int getConfig(char *Config, char *buff, char *ConfigFile);
int get_SerialConf(char *baudrate, char *parity, char *data_bit, char *stop_bit);
int TcpServer_Mode();
int TcpClient_Mode();
int UdpServer_Mode();
int Udp_Client_Mode();

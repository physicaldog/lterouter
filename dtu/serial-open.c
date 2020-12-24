/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

#define SerPort "/dev/ttymxc1"
#define BAUDRATE 9600
#define DATA_BIT 8
#define PARITY 'N'
#define STOP_BIT 1
#define Vmin 255
#define Vtime 1
*/
#include "suyi_dtu.h"

void set_timeOut(struct termios *termptr, unsigned int min, unsigned int time)
{
	log_msg("********%s********\n",__FUNCTION__);	
	termptr->c_cc[VMIN] = min;
	termptr->c_cc[VTIME] = time;
	
}

void set_stopBit(struct termios *termptr, unsigned int stopBit)
{
	log_msg("********%s********\n",__FUNCTION__);	

	switch(stopBit) {
		case 2:
			termptr->c_cflag |= CSTOPB;
			break;
		default:
			termptr->c_cflag &= ~CSTOPB;
	}
	
}
void set_parity(struct termios *termptr, unsigned char parity)
{
	log_msg("********%s********\n",__FUNCTION__);	

	switch(parity) {
		case 'n':
		case 'N':
			termptr->c_cflag &= ~PARENB;
			break;
		case 'e':
		case 'E':
			termptr->c_cflag |= PARENB;
			termptr->c_cflag &= ~PARODD;
			break;
		case 'o':
		case 'O':
			termptr->c_cflag |= PARENB;
			termptr->c_cflag |= PARODD;
			break;
		default:
			termptr->c_cflag &= ~PARENB;
			break;
	}

}

void set_data_bit(struct termios *termptr, unsigned int data_bit)
{
	log_msg("********%s********\n",__FUNCTION__);	

	termptr->c_cflag &= ~CSIZE;
	switch(data_bit) {
		case 5:
			termptr->c_cflag |= CS5;
			break;
		case 6:
			termptr->c_cflag |= CS6;
			break;
		case 7:
			termptr->c_cflag |= CS7;
			break;
		case 8:
			termptr->c_cflag |= CS8;
			break;
		default:
			termptr->c_cflag |= CS8;
			break;
	}
}

void set_baudrate(struct termios *termptr, unsigned int baudrate)
{
	int ret = 0;
	log_msg("********%s********\n",__FUNCTION__);	
	switch(baudrate) {
	case 1200:
		cfsetispeed(termptr,B1200);
		break;
	case 2400:
		cfsetispeed(termptr,B2400);
		break;
	case 4800:
		cfsetispeed(termptr,B4800);
		break;
	case 9600:
		cfsetispeed(termptr,B9600);
		break;
	case 19200:
		cfsetispeed(termptr,B19200);
		break;
	case 38400:
		cfsetispeed(termptr,B38400);
		break;
	case 57600:
		cfsetispeed(termptr,B57600);
		break;
	case 115200:
		cfsetispeed(termptr,B115200);
		break;
	}
}

void set_mode(struct termios *termptr)
{
	log_msg("********%s********\n",__FUNCTION__);	

	termptr->c_iflag  &= ~(IXON | IXOFF | IXANY); 
	termptr->c_oflag  &= ~OPOST; 
	termptr->c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); ; 

}
//参数需改成文件读取方式
void setTermios(struct termios *termptr,char *baudrate,char *parity,char *data_bit,char *stop_bit)
{
	 int ret = 0;

	log_msg("********%s********\n",__FUNCTION__);	
/*
	ret = getConfig("baudrate",baudrate,ConfigFile);
	if(0 > ret){
		strcpy(baudrate,"9600");
	}
	ret = getConfig("parity",parity,ConfigFile);
	if(0 > ret){
		strcpy(parity,"N");
	}
	ret = getConfig("data_bit",data_bit,ConfigFile);
	if(0 > ret){
		strcpy(data_bit,"8");
	}
	ret = getConfig("stop_bit",stop_bit,ConfigFile);
	if(0 > ret){
		strcpy(stop_bit,"1");
	}
	
*/
	log_msg("%ld,%c,%d,%d\n",atol(baudrate),parity[0],atoi(data_bit),atoi(stop_bit));
	set_mode(termptr);
	set_baudrate(termptr,atoi(baudrate));
	set_data_bit(termptr, atoi(data_bit));
	set_parity(termptr, parity[0]);
	set_stopBit(termptr, atoi(stop_bit));

	//字符间隔时间超过100ms时返回，判定为一桢结束。默认一桢数据长度不超过255个字符。
	set_timeOut(termptr,Vmin,Vtime);

}

void serRead(int fd, char *buffer, unsigned int len)
{
	int i = 0;
	int ret = 0;

	log_msg("********%s********\n",__FUNCTION__);	

	ret = read(fd,buffer,len);
	if(0 > ret)	
		log_msg("serRead failed!\n");
	else{
		for(i;i<ret;i++){
			log_msg("%c ",buffer[i]);
		}
	}
}

void serWrite(int fd, char *buffer, unsigned int len)
{
	int ret = 0;

	log_msg("********%s********\n",__FUNCTION__);	

	if(0 >= fd){
		log_ret("fd error");
		return;
	}
	ret = write(fd,buffer,len);
	if(0 > ret)	
		log_msg("serWrite failed!\n");
}

int openSer(char *SerDev,char *baudrate,char *parity,char *data_bit,char *stop_bit)
{
	int ret = 0;
	int fd = 0;
	struct termios termptr;
	log_msg("********%s********\n",__FUNCTION__);	
	//fd = open(SerPort,O_RDWR|O_NOCTTY|O_NDELAY);//加上O_NDELAY read函数不阻塞
	fd = open(SerDev,O_RDWR|O_NOCTTY);
	if(-1 == fd){
		log_ret("can not open dev");
		return -1;
	}
	ret = tcgetattr(fd,&termptr);
	if(-1 == ret){
		log_ret("can not get termios attr");
		return -1;
	}
	setTermios(&termptr,baudrate,parity,data_bit,stop_bit);
	ret = tcsetattr(fd,TCSANOW,&termptr);
	if(-1 == ret){
		log_ret("can not set termios attr");
		return -1;
	}
	return fd;
}
#if 1
int main(int argc, char* argv[])
{
	int ret = 0;
	int i = 0;
	int fd = 0;
	char buffer_0[8] = {0x03,0x05,0x00,0x00,0x00,0x00,0xCC,0x28};
	char buffer_1[8] = {0x03,0x05,0x00,0x00,0xFF,0x00,0x8D,0xD8};
	struct termios termptr;

	log_msg("********%s********\n",__FUNCTION__);
	if(argc < 2){
		printf("需要参数:0表示闭合继电器；1表示断开继电器");
		return;
	}
	fd = openSer(SerPort,"9600","N","8","1");
	if(0 >= fd){
		log_ret("openSer failed!");
		return -1;
	}
	if(!strcmp(argv[1],"1")){
		serWrite(fd,buffer_1,8);
		sleep(2);
		memset(buffer_1,0,8);
		//serRead(fd,buffer_1,8);
		//for(i=0;i<8;i++)
			//printf("%04x ",buffer_1[i]);
	}else{
		serWrite(fd,buffer_0,8);
		sleep(2);
		memset(buffer_0,0,8);
		//serRead(fd,buffer_0,8);
		//for(i=0;i<8;i++)
			//printf("%04x ",buffer_0[i]);
	}
	close(fd);
	return 0;

}
#endif

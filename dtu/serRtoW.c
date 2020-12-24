#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

#define SerPort "/dev/ttymxc2"
#define BAUDRATE 115200
#define DATA_BIT 8
#define PARITY 'N'
#define STOP_BIT 1
#define Vmin 255
#define Vtime 1

void set_timeOut(struct termios *termptr, unsigned int min, unsigned int time)
{
	printf("********%s********\n",__FUNCTION__);	
	termptr->c_cc[VMIN] = min;
	termptr->c_cc[VTIME] = time;
	
}

void set_stopBit(struct termios *termptr, unsigned int stopBit)
{
	printf("********%s********\n",__FUNCTION__);	

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
	printf("********%s********\n",__FUNCTION__);	

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
	printf("********%s********\n",__FUNCTION__);	

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
	printf("********%s********\n",__FUNCTION__);	
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
	printf("********%s********\n",__FUNCTION__);	

	termptr->c_iflag  &= ~(IXON | IXOFF | IXANY); 
	termptr->c_oflag  &= ~OPOST; 
	termptr->c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); ; 

}
//参数需改成文件读取方式
void setTermios(struct termios *termptr,char *ConfigFile)
{
	printf("********%s********\n",__FUNCTION__);	

	set_mode(termptr);
	set_baudrate(termptr, BAUDRATE);
	set_data_bit(termptr, DATA_BIT);
	set_parity(termptr, PARITY);
	set_stopBit(termptr, STOP_BIT);

	//字符间隔时间超过100ms时返回，判定为一桢结束。默认一桢数据长度不超过255个字符。
	set_timeOut(termptr,Vmin,Vtime);

}

void serRead(int fd, char *buffer, unsigned int len)
{
	int i = 0;
	int ret = 0;

	printf("********%s********\n",__FUNCTION__);	

	ret = read(fd,buffer,len);
	if(0 > ret)	
		printf("serRead failed!\n");
	else{
		for(i;i<ret;i++){
			printf("%c ",buffer[i]);
		}
	}
}

void serWrite(int fd, char *buffer, unsigned int len)
{
	int ret = 0;

	printf("********%s********\n",__FUNCTION__);	

	if(0 >= fd){
		perror("fd error");
		return;
	}
	ret = write(fd,buffer,len);
	printf("write :%s,%d\n",buffer,ret);
	if(0 > ret)	
		printf("serWrite failed!\n");
}

int openSer(char *SerDev)
{
	int ret = 0;
	int fd = 0;
	struct termios termptr;
	printf("********%s********\n",__FUNCTION__);	
	//fd = open(SerPort,O_RDWR|O_NOCTTY|O_NDELAY);//加上O_NDELAY read函数不阻塞
	fd = open(SerDev,O_RDWR|O_NOCTTY);
	if(-1 == fd){
		perror("can not open dev");
		return -1;
	}
	ret = tcgetattr(fd,&termptr);
	if(-1 == ret){
		perror("can not get termios attr");
		return -1;
	}
	setTermios(&termptr,NULL);
	ret = tcsetattr(fd,TCSANOW,&termptr);
	if(-1 == ret){
		perror("can not set termios attr");
		return -1;
	}
	return fd;
}
#if 1
int main()
{
	int ret = 0;
	int fd = 0;
	char buffer[1024] = {"monkey"};
	struct termios termptr;

	printf("********%s********\n",__FUNCTION__);
	//fd = open(SerPort,O_RDWR|O_NOCTTY|O_NDELAY);//加上O_NDELAY read函数不阻塞
	fd = openSer(SerPort);
	if(0 >= fd){
		perror("openSer failed!");
		return -1;
	}

	while(1){
		serRead(fd,buffer,255);
		printf("recv:%s,%d\n",buffer,strlen(buffer));
		serWrite(fd,buffer,strlen(buffer));
		//serWrite(fd,"\n",1);
		memset(buffer,0,1024);
	}	

	close(fd);
	return 0;

}
#endif

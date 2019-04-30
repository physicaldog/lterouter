#include "common.h"
#include "at.h"

#define SerPort	"/dev/ttyem300"

char at_arr[16][64] = {/*{{{*/
	"AT^SYSINFOEX\r\n", 
	"AT^NDISSTATQRY?\r\n",
	"AT^NDISDUP=1,1,\"cmnet\"\r\n",
	"AT^NDISDUP=1,0,",
};/*}}}*/

static int openDev(char *Dev)
{/*{{{*/
	int iFd;

	struct termios opt; 

	//iFd = open(Dev, O_RDWR | O_NOCTTY | O_NONBLOCK);                        
	iFd = open(Dev, O_RDWR | O_NOCTTY);                        
	if(iFd < 0) {
		return -1;
	}

	tcgetattr(iFd, &opt);      

	//cfsetispeed(&opt, B57600);
	//cfsetospeed(&opt, B57600);

	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);


	opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG | ECHOE);
	opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
	opt.c_oflag   &=   ~(OPOST);
	opt.c_cflag   &=   ~(CSIZE   |   PARENB);
	opt.c_cflag   |=   CS8;

	opt.c_cc[VMIN]   =   DATA_LEN;                                      
	opt.c_cc[VTIME]  =   100;

	if (tcsetattr(iFd,   TCSANOW,   &opt)<0) {
		return   -1;
	}


	return iFd;
}/*}}}*/

//多线程
void status_read(int fd)
{/*{{{*/
	char rbuff[4096] = {0};
	char* ptr = rbuff;
	int ret=0;
	FILE *fp = NULL;
    char system_time[64] = {0};

	fp = popen("date +\"%Y-%m-%d %H:%M:%S\"","r");
	if(fp){
		ret = fread(system_time,1,sizeof(system_time),fp);
		fclose(fp);
	}
	while(1){
		ret = read(fd, ptr, 1);
		if(ret == 1 && (*ptr == '\n')){
			if(strstr(rbuff,"SYSINFOEX:")){
				break;
			}
			else{
				memset(rbuff,0,strlen(rbuff));
				ptr = rbuff;
				continue;
			}
		}
		else{
			ptr++;
		}
	}
	printf("%s",system_time);
	printf("%s",rbuff);


}/*}}}*/

void send_at(int fd)
{/*{{{*/
	int ret;
	ret = write(fd, at_arr[0], strlen(at_arr[0]));
	return;

}/*}}}*/

int main(void) 
{
	int i=0,ret=0,fd=0;
	int reboottime = 0;
	char buff[64] = {0};
	
	
	fd = openDev(SerPort);
	if(0 >= fd){
		return -1;
	}
	printf("GetSysinfoex Start:\n");
	while(1){
		send_at(fd);
		sleep(1);
		status_read(fd);
		
	}

	close(fd);

	return 0;
}

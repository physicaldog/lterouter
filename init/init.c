#include "common.h"
#include "at.h"

#define DEBUG                 1
#define DATA_LEN                0xFF                                    /* test data's len              */
#define APN                     "\"cmnet\"\r\n"
#define SerPort "/dev/ttyUSB2"

char at_arr[16][64] = {/*{{{*/
        "AT^SYSINFOEX\r\n", 
        "AT^NDISSTATQRY?\r\n",
        "AT^NDISDUP=1,1,\"cmnet\"\r\n",
        "AT^NDISDUP=1,0,",
};/*}}}*/

#if 0
int openDev(char *Dev)
{/*{{{*/
        int fd = 0, ret=0;

        fd = open(Dev, O_RDWR | O_NOCTTY);
        ret = set_Parity(fd, 115200, 8, 1, 'N');
        if(0 > ret){
                perror("set_Parity Failed!\n");
                return -1;
        }

        return fd;
}/*}}}*/

int set_Parity(int fd,int speed, int databits,int stopbits,int parity)
{ /*{{{*/

        struct termios options; 
        if  ( tcgetattr( fd,&options)  !=  0) { 
                perror("SetupSerial 1");     
                return -1;  
        }

        /*set speed*/
        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);

        options.c_cflag &= ~CSIZE; 

        switch (databits) /*设置数据位数*/
        {   
                case 7:        
                        options.c_cflag |= CS7; 
                        break;
                case 8:     
                        options.c_cflag |= CS8;
                        break;   
                default:    
                        fprintf(stderr,"Unsupported data size\n"); 
                        return -1;  
        }
        switch (parity) 
        {   
                case 'n':
                case 'N':    
                        options.c_cflag &= ~PARENB;   /* Clear parity enable */
                        options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
                        break;  
                case 'o':   
                case 'O':     
                        options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
                        options.c_iflag |= INPCK;             /* Disnable parity checking */ 
                        break;  
                case 'e':  
                case 'E':   
                        options.c_cflag |= PARENB;     /* Enable parity */    
                        options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
                        options.c_iflag |= INPCK;       /* Disnable parity checking */
                        break;
                case 'S': 
                case 's':  /*as no parity*/   
                        options.c_cflag &= ~PARENB;
                        options.c_cflag &= ~CSTOPB;break;  
                default:   
                        fprintf(stderr,"Unsupported parity\n");    
                        return -1;  
        }  
        /* 设置停止位*/  
        switch (stopbits)
        {   
                case 1:    
                        options.c_cflag &= ~CSTOPB;  
                        break;  
                case 2:    
                        options.c_cflag |= CSTOPB;  
                        break;
                default:    
                        fprintf(stderr,"Unsupported stop bits\n");  
                        return -1; 
        } 
        /* Set input parity option */ 
        if (parity != 'n')   
                options.c_iflag |= INPCK; 

        /*set RAW MODE*/
        options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
        options.c_oflag  &= ~OPOST;   /*Output*/

        tcflush(fd,TCIFLUSH);
        options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/   
        options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
        if (tcsetattr(fd,TCSANOW,&options) != 0)   
        { 
                perror("SetupSerial 3");   
                return -1;  
        } 
        return 0;  
}/*}}}*/

#else
static int openDev(char *Dev)
{/*{{{*/
        int iFd;

        struct termios opt; 

        //iFd = open(Dev, O_RDWR | O_NOCTTY | O_NONBLOCK);                        
        iFd = open(Dev, O_RDWR | O_NOCTTY);                        
        if(iFd < 0) {
                perror(Dev);
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
        opt.c_cc[VTIME]  =   150;

        if (tcsetattr(iFd,   TCSANOW,   &opt)<0) {
                return   -1;
        }


        return iFd;
}/*}}}*/
#endif

#if 0 //单线程 + 定时器
void status_read()
{/*{{{*/
        char rbuff[4096] = {'\0'};
        char* ptr = rbuff;
        int ret=0;
        //write(fd, at_arr[1], strlen(at_arr[1]));
        while (1) {
                //printf("start read\n");
                ret = read(fd, ptr, 0x01);
                //printf("read ret =%d\n",ret);
                if (1 == ret) {
                        if ('\n' != *ptr) {
                                ptr++;
                        }
                        else {
                                printf("%s",rbuff);
                                direct_process(rbuff);
                                memset(rbuff,0,strlen(rbuff));
                                ptr = rbuff;
                        }
                }
                else {
                        perror("read err:");
                        return;
                }
        }
        printf("read exit");
}/*}}}*/

void send_at()
{/*{{{*/
        int ret;

        alarm(2);

        ret = write(fd, at_arr[0], strlen(at_arr[0]));
        if(ret < 0){
                perror("send_at");
                return;
        }
        return;

}/*}}}*/

int main(void) 
{/*{{{*/
        int i=0,ret=0;
        pthread_t rThread, tThread;

        fd = openDev(SerPort);
        printf("open \n");
        if(0 >= fd){
                perror("OPEN /dev/ttyUSB2 FAILED!");
                return -1;
        }

        signal(SIGALRM, send_at);
        alarm(2);
        status_read();
        close(fd);

        return 0;
}/*}}}*/
#else //多线程
void status_read(int *fptr)
{/*{{{*/
        char rbuff[4096] = {'\0'};
        char* ptr = rbuff;
        int ret=0;
        fd = *fptr;
        while (1) {
                ret = read(*fptr, ptr, 0x01);
                if (1 == ret) {
                        if ('\n' != *ptr) {
                                ptr++;
                        }
                        else {
                                printf("%s",rbuff);
                                direct_process(rbuff);
                                memset(rbuff,0,strlen(rbuff));
                                ptr = rbuff;
                        }
                }
                else {
                        perror("read err:");
                        return;
                }
        }
        printf("read exit");
}/*}}}*/

void send_at(int *fptr)
{/*{{{*/
        int ret;

        while(1){
                ret = write(*fptr, at_arr[0], strlen(at_arr[0]));
                if(ret < 0){
                        perror("send_at");
                        return;
                }
                sleep(5);
        }
        return;

}/*}}}*/


int main(void) 
{
        int i=0,ret=0,fd=0;
        pthread_t rThread, tThread;

        fd = openDev(SerPort);
        printf("open \n");
        if(0 >= fd){
                perror("OPEN /dev/ttyUSB2 FAILED!");
                return -1;
        }

//        signal(SIGALRM, send_at);
//        alarm(2);
        ret = pthread_create(&rThread,NULL,(void *)status_read,(void *)&fd);
        if(0 != ret){
            perror("rThread create failed!\n");
            return -1;
        }
        ret = pthread_create(&tThread,NULL,(void *)send_at,(void *)&fd);
        if(0 != ret){
            perror("tThread create failed!\n");
            return -1;
        }

        ret = pthread_join(rThread,NULL);
        ret = pthread_join(tThread,NULL);

        close(fd);

        return 0;
}

#endif

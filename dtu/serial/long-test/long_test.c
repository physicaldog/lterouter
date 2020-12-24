/*********************Copyright(c)******************************
**
**          Guangzhou ZHIYUAN electronics Co.,LTD
**
**              http://www.zlg.cn
**
** File:					serial/long-test.c
** Created Date:			2008-05-19
** Latest Modified Date:	2015-12-15
** Latest Version:			v1.0
** Description:             NONE
**
****************************************************************/
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define DATA_LEN	0x08		/* test data's len              */
#define STOP		0			/* test stop                    */
#define RUN			1			/* test running                 */
#define SERIAL_SEND	2			/* RX2 and TX2					*/
#define SERIAL_REC	3			/* RX3 and TX3					*/

static int GiRunStatue;			/* test pthread status          */

#define DEBUG                 1

static int GiSerialFds[4] = {-1, -1, -1, -1};		/* all serial device fd         */

/************************************************************
** Function name:		reportString
** Descriptions:		write string to file "report.txt",if 
**						the file not exit,the function will 
**						create it
** ARGUMENTS:
** input paramters:		cStr	String will be writed to file
** output paramters:	NONE
** Return value:		NONE
*************************************************************/
static void reportString(char *cStr)
{
    int iFd;

    iFd = open("report.txt", O_APPEND | O_RDWR | O_CREAT, 
                S_IRUSR | S_IWUSR |  S_IRGRP | S_IROTH);
   
    write(iFd, cStr, strlen(cStr));

    close(iFd);
}

/************************************************************
** Function name:		openSerial
** Descriptions:		open serial port at raw mode
** ARGUMENTS:
** input paramters:		iNum	serial port number
** output paramters:	NONE
** Return value:		iFD		file descriptor
*************************************************************/
static int openSerial(int iPort)
{
    int iFd;

    struct termios opt; 
    char cSerialName[15];

    if (iPort >= 8) {
        printf("no such serial port:ttymxc%d . \n", iPort-1);
        exit(1);
    }
    
    if (GiSerialFds[iPort-1] > 0) {
        return GiSerialFds[iPort-1];
    }

    sprintf(cSerialName, "/dev/ttymxc%d", iPort-1);
    printf("open serial name:%s \n", cSerialName);
    iFd = open(cSerialName, O_RDWR | O_NOCTTY);                        
    if(iFd < 0) {
        perror(cSerialName);
        return -1;
    }

    tcgetattr(iFd, &opt);      

    cfsetispeed(&opt, B115200);
    cfsetospeed(&opt, B115200);
    
    /*
     * raw mode
     */
    opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    opt.c_oflag &= ~(OPOST);
    opt.c_cflag &= ~(CSIZE | PARENB);
    opt.c_cflag |= CS8;

    /*
     * 'DATA_LEN' bytes can be read by serial
     */
    opt.c_cc[VMIN] = DATA_LEN;                                      
    opt.c_cc[VTIME] = 0;

    if (tcsetattr(iFd, TCSANOW, &opt)<0) {
        return   -1;
    }

    GiSerialFds[iPort - 1] = iFd; 

    return iFd;
}

/************************************************************
** Function name:		setData
** Descriptions:		init test-data with rand data
** ARGUMENTS:
** input paramters:		ucValues	buff which data need inited
**						iLen		data's len in buff
** output paramters:	ucValues	buff which data have inited
** Return value:		NONE
*************************************************************/
static void setData(unsigned char *ucValues, int iLen)
{
    int i;
    static int iGerm;

    if (ucValues == NULL) {
        printf("error: uninit pointer: ucValues in serial/long-test.c::setData. \n");
    }

    srand(iGerm);
    iGerm++;
    
    for (i = 0; i < iLen; i++) {
        ucValues[i] = i%0xff; 
    }
}

/************************************************************
** Function name:		testSerial
** Descriptions:		This function test two serial port,
**						one port send data, and the other 
**						recevie data.Will test 20 times.
** ARGUMENTS:
** input paramters:		iFdsend		serial port which send data
**						iFdRec		serial port which receive data
**						iTimes		times of test has done
** output paramters:	NONE
** Return value:		0			test success
**						others		test faile
*************************************************************/
static struct timeval tv_timeout;
static fd_set fs_read;
static int testSerial(int iPortSend, int iPortRec, int iTimes)
{
    unsigned char ucDataSend[DATA_LEN], ucDataGet[DATA_LEN];
    int iRet = 1; 
    int i, j;

    char cStr[150];
    time_t  timeP;
    int iFdSend, iFdRec;

    /*
     * get descriptor of serial port while will send data
     */
    iFdSend = openSerial(iPortSend);
    /*
     * get descriptor of serial port while will receive data
     */
    iFdRec  = openSerial(iPortRec);
    /*
     * test serial port send/receive for 20 times
     */
    for (i = 0; i < 2; i++) {

        setData(ucDataSend, DATA_LEN);			/* init send data randily       */
        write(iFdSend, ucDataSend, DATA_LEN);	/* send data to other serial    */
        /*
         *  get data at the other serial port,
         *  if time out for 2 seconds, report error
         */ 
        FD_ZERO(&fs_read);
        FD_SET (iFdRec, &fs_read);

        tv_timeout.tv_sec = 0;
        tv_timeout.tv_usec = 2000000;
        iRet = select(iFdRec + 1, &fs_read, NULL, NULL, &tv_timeout); 

        if (iRet) {
            iRet = read(iFdRec, ucDataGet, DATA_LEN);		/* receive data                 */
            /*
             *  compare send data and receive data. if send data and receive are unequal,
             *  report error.
             */ 
            for (j = 0; j < DATA_LEN; j++) {
				int i = 0;
                if (ucDataSend[j] != ucDataGet[j]) {
                    printf(" data error ,send port: %d receive port:%d \n", iPortSend, iPortRec);

                    sprintf(cStr, "\ndate test error, send port: %d  receive port: %d \n",
                            iPortSend, iPortRec);
                    reportString(cStr);

                    sprintf(cStr, "at times: %d \n", iTimes);
                    reportString(cStr);

                    sprintf(cStr, "error time: %s \n ", asctime(gmtime(&timeP)) );
                    reportString(cStr);
#ifdef DEBUG
				    printf("get data:%d \n", iRet); 
					for (i = 0; i < iRet; i++) {
						if (i%15 == 0) 
							printf("\n");
						printf(" %x %x,", ucDataSend[i], ucDataGet[i]);
					}
#endif
                    return -1;
                }
            }

        } else {
            /*
             *  if time out, report error
             */

            printf("time out send port: %d receive port: %d \n", iPortSend, iPortRec);

            sprintf(cStr, "\ntime out error, send port: %d  receive port: %d \n",
                            iPortSend, iPortRec);
            reportString(cStr);

            sprintf(cStr, "at times: %d \n", iTimes);
            reportString(cStr);

            sprintf(cStr, "error time: %s \n ", asctime(gmtime(&timeP)) );
            reportString(cStr);

            return -2;
        }
    }

    return 0;
}

/************************************************************
** Function name:		stopTest
** Descriptions:		wait for user input "stop" to stop test.
**						when user input "stop", the function will 
**						set GiRunStatue as STOP, and the serial
**						long times test pthread will be terminated.
** input paramters:		iParam		private data
** output paramters:	NONE
** Return value:		0			success
**						other		faile
*************************************************************/
static int stopTest(int *iParam)
{
    char cStr[20];

    while(strcmp(cStr, "stop") != 0) {
        scanf("%s", cStr);
    }       

    printf("serial test finish.... \n");

    GiRunStatue = STOP;

    return 0;
}

/************************************************************
** Function name:           main
** Descriptions:            main function
** input paramters:         NONE
** output paramters:        NONE
** Return value:            0       success
**                          other   faile
*************************************************************/
char Usage[] = {
"**********************************************\r\n \
Usage:\r\n \
connect RX2 to TX3,RX3 to TX2 \r\n \
then use ./long-test to run the demo \r\n \
input 'stop' or 'ctl+c' to quit \r\n \
***********************************************\r\n \
"
};
int main(void)
{
    int iRet1, iRet2;
    int i, iSuccess, iFaile;
    pthread_t threadNew;
    int iT = 0;

    char    cStr[150];
    time_t  timeP;
    time_t  timeUse;
    time_t  timeStart;
    struct  tm *p;
    printf(Usage);
    /*
     *  make a report's title and report the test start time
     */
    reportString("\n\n********************************************* \n\n");
    time(&timeP);
    time(&timeStart);
    sprintf(cStr, "start time: %s \n", asctime(gmtime(&timeP)) );
    reportString(cStr);
    /*
     *  open serial device file needed tested.
     */
    openSerial(SERIAL_SEND);
    openSerial(SERIAL_REC);
    /*
     *  clean the test status flag
     */
    iSuccess = 0;
    iFaile   = 0;
    /*
     *   create a pthread which waite for user input "stop" and stop the test
     */
    GiRunStatue = RUN;
    iRet1 = pthread_create(&threadNew, PTHREAD_CREATE_JOINABLE, (void *)stopTest, NULL);
    if (iRet1) {
        perror("pthread_creae: threadNew. \n");
        return 0;
    }

    i = 0;
    while(1) {
        i++;
        iRet1 = testSerial(SERIAL_SEND, SERIAL_REC, i);
        iRet2 = testSerial(SERIAL_REC, SERIAL_SEND, i);

        /*
         *  if all serial test pass, test success flag count    
         *  else test faile flag count
         */
        if ((iRet1 + iRet2) == 0) {                   
           iSuccess++; 
        } else {
			iFaile++;
			if (iFaile > 10)  {
				printf("O no ! faile!~~! \n");
				return -1;	
			}
        }

        /*
         *  tell user how the test going
         */
        printf("serial test %d times,failed %d times \n",i, iFaile);

        /*
         *  tell user how much time pass
         */
        time(&timeP);
        timeUse = timeP - timeStart;
        p = gmtime(&timeUse);
        printf("use time: %d-%d %d:%d:%d \n", p->tm_mon, p->tm_mday - 1, 
                                    p->tm_hour, p->tm_min, p->tm_sec);
        /*
         *  report the test result every hour
         */
        if (iT != p->tm_hour) {
            iT = p->tm_hour;

            sprintf(cStr, "\nsuccess times: %d", iSuccess);
            reportString(cStr);

            sprintf(cStr, "\nfaile times:   %d", iFaile);
            reportString(cStr);

            time(&timeP);
            sprintf(cStr, "\nend time: %s ", asctime(gmtime(&timeP)) );
            reportString(cStr);

            time(&timeP);
            timeUse = timeP - timeStart;
            p = gmtime(&timeUse);
            sprintf(cStr, "\nuse time: %d-%d %d:%d:%d \n", p->tm_mon, p->tm_mday - 1, 
                                    p->tm_hour, p->tm_min, p->tm_sec);
            reportString(cStr);
        }

        /*
         *  finish the test if user have input "stop"
         */
        if (GiRunStatue == STOP) {
            break;
        }
    }

    /*
     * report the test result
     */
    sprintf(cStr, "\nsuccess times: %d", iSuccess);
    reportString(cStr);

    sprintf(cStr, "\nfaile times:   %d", iFaile);
    reportString(cStr);

    time(&timeP);
    sprintf(cStr, "\nend time: %s ", asctime(gmtime(&timeP)) );
    reportString(cStr);

    time(&timeP);
    timeUse = timeP - timeStart;
    p = gmtime(&timeUse);
    sprintf(cStr, "\nuse time: %d-%d %d:%d:%d \n", p->tm_mon, p->tm_mday - 1, 
                                    p->tm_hour, p->tm_min, p->tm_sec);
    reportString(cStr);


    reportString("\n---------------------------------------------\n");

    printf("please check the test report at report.txt \n");
    
    return 0;
}


/************************************************************
						end file
*************************************************************/

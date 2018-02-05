#include "common.h"
#include "at.h"
#define APN "\"cmnet\"\r\n" //可不接\n。

char *at[8] = {
    //"AT^NDISDUP=1,1,\"jsdl.34njnw\"\r\n",
    //"AT^NDISDUP=1,1,\"jsdl.34njnw\"\r\n",
    //"AT^NDISDUP=1,1,\"jsepc4g.js\"\r\n",
    //"AT^NDISDUP=1,1,\"cmnet\"\r\n",
    "AT^NDISDUP=1,1,",
    //"AT^NDISDUP=1,1,\"tdtech\"\r\n",
    "AT^NDISSTATQRY?\r\n",
    };

/*********************************************************************************************************
** Function name:           openSerial
** Descriptions:            open serial port at raw mod
** input paramters:         iNum        serial port which can be value at: 1, 2, 3, 4
** output paramters:        NONE
** Return value:            file descriptor
** Create by:               zhuguojun
** Create Data:             2008-05-19
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void direct_process(char *buff)
{
    char *ptr = NULL;
    char *at_arr[16] = {
        "SRVST",
        "NDISSTAT",
        "SYSINFOEX",
        "NDISSTATQRY",
    };
    if ('^' == buff[0]) {
        printf("get direct:%s\n",buff);
        //ptr = strstr(buff,at_arr[0]);
        if ((strstr(buff,at_arr[0])) || (strstr(buff,at_arr[2]))) {
            get_sysinfoex(buff);
            return;
        }
        //ptr = strstr(buff,at_arr[1]);
        if (strstr(buff,"IPV4") && ((strstr(buff,at_arr[1])) || (strstr(buff,at_arr[3]))) ) {
            get_ndisstat(buff);
            return;
        }

    }
}

/*********************************************************************************************************
** Function name:           openSerial
** Descriptions:            open serial port at raw mod
** input paramters:         iNum        serial port which can be value at: 1, 2, 3, 4
** output paramters:        NONE
** Return value:            file descriptor
** Create by:               zhuguojun
** Create Data:             2008-05-19
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void get_sysinfoex(char *buff)
{
    char *ptr = NULL;
    
    ptr = strstr(buff," ");
    if(NULL == ptr)
        return;
    if ('2' == ptr[1]){
        printf("有效网络\n");
        write(fd,at[1],strlen(at[1]));
        net_sta = 1;
    }
    else{
        printf("无效网络\n");
        //alarm(2);
        net_sta = 0;
    }
}

/*********************************************************************************************************
** Function name:           openSerial
** Descriptions:            open serial port at raw mod
** input paramters:         iNum        serial port which can be value at: 1, 2, 3, 4
** output paramters:        NONE
** Return value:            file descriptor
** Create by:               zhuguojun
** Create Data:             2008-05-19
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void get_ndisstat(char *buff)
{
        char *ptr = NULL;
        FILE *fe = NULL;
        int ret = 0;
        int file = 0;
        char ip[64] = {'\0'};
        char rbuff[64] = {'\0'};//读取文件缓存
        char direct[64] = {'\0'};//


        ptr = strstr(buff," ");
        if(NULL == ptr)
                return;
        switch (ptr[1]){
                case '0':
                        printf("连接断开\n");
                        ndis_sta = 0;
                        if((1 == net_sta)){
                                printf("开始拨号\n");
#if 1 
                                //从配置文件中获取apn;                   
                                fe = fopen("/opt/config/netconfig","r");
                                if (NULL == fe){
                                        perror("netconfig open failed!\n");
                                        return;
                                }
                                while(fgets(rbuff,sizeof(rbuff),fe)){
                                        if(strstr(rbuff,"apn:"))
                                        {   
                                                file = 1;
                                                strcat(direct,"AT^NDISDUP=1,1,\"");
                                                rbuff[strlen(rbuff)-1] = '\0';
                                                strcat(direct,rbuff+strlen("apn:"));
                                                strcat(direct,"\"\r\n");
                                                printf("dir = %s",direct);
                                                break;
                                        }
                                        else
                                                memset(rbuff,'\0',sizeof(rbuff));
                                }
                                fclose(fe);
                                if(0 == file)
                                    printf("未配置apn\n"); 
                                write(fd,"\r\n",strlen("\r\n"));
                                write(fd,direct,strlen(direct));
#else                    
                                write(fd,at[0],strlen(at[0]));
                                write(fd,APN,strlen(APN));
#endif
                        }
                        break;
                case '1'://获取ip
                        printf("有效链接\n");//已拨号
                        if(0 == ecm_done){
                                printf("获取ip\n");
                                system("bash /opt/init/netconf.sh");
                                ret = get_local_ip("usb0",ip);
                                if(0 != strcmp(ip,""))
                                    ecm_done = 1;
                                else{
                                    ecm_done = 0;
                                    printf("未获取到ip\n");
                                    }//如何检测系统的网络链接
                        }
                        //ecm_done = 0;
                        ndis_sta = 1;
                        break;
                case '2':
                        break;
                case '3':
                        break;
        }
}

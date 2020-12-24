#include "suyi_dtu.h"
#include "./uci.h" 

int get_config(char *package,char *section,char *option,char *buff) 
{ 
	struct uci_context *c; 
	struct uci_ptr p; 
	char path[64] = {0};

	sprintf(path,"%s.%s.%s",package,section,option); 

	c = uci_alloc_context(); 
	uci_set_confdir(c, "/opt/config");

	if((UCI_OK == uci_lookup_ptr(c, &p, path, true)) && (p.flags & UCI_LOOKUP_COMPLETE)) { 

		strcpy(buff,p.o->v.string);
	} 
	else{
		printf("%s not found! ",path);
		uci_perror(c, "\n"); 
		return -1; 
	}

	uci_free_context(c); 
	return(0); 
}

int getConfig(char *Config, char *buff, char *ConfigFile)
{
    FILE *fe = NULL;
    int fexist = 0;
	char *ptr = NULL;
    char rbuff[64] = {'\0'};//读取文件缓存

    fe = fopen(ConfigFile,"r+");
    if (NULL == fe){
            log_msg("no netconfig file\n");
            return -1;//未找到配置文件
    }
	
	if(0 == strlen(Config))
	{
		log_msg("Config is NULL\n!");
	}

    while (fgets(rbuff,sizeof(rbuff),fe)){
            if (strstr(rbuff,Config)){
                    fexist = 1;
                    break;
                    }
            else
                    memset(rbuff,'\0',sizeof(rbuff));
    }
    fclose(fe);
    if(fexist)
	{
		ptr = strchr(rbuff,':');
		
        strcat(buff,(++ptr));
		while(ptr=strchr(buff,'\n'))
			*ptr = '\0';
	}
    else
        return -2;//未配置
    
    return 0;
}

int get_SerialConf(char *baudrate, char *parity, char *data_bit, char *stop_bit)
{
	
	log_msg("\n********%s********\n",__FUNCTION__);
	log_open("suyi_dtu", LOG_CONS | LOG_PID, LOG_LOCAL2);

	log_msg("This is dtu log_msg!\n");
	log_msg("*********************************************\n");
	//getConfig("baudrate",baudrate,DtuConf);
	get_config("config","dtu","baudrate",baudrate);
	//getConfig("parity",parity,DtuConf);
	get_config("config","dtu","parity",parity);
	//getConfig("data_bit",data_bit,DtuConf);
	get_config("config","dtu","data_bit",data_bit);
	//getConfig("stop_bit",stop_bit,DtuConf);
	get_config("config","dtu","stop_bit",stop_bit);

	return 0;
}
int main()
{
	int ret = 0;
	char mode[8] = {0};

	log_msg("\n********%s********\n",__FUNCTION__);
	
	get_config("config","dtu","mode",mode);
	//getConfig("mode",mode,DtuConf);

	switch (mode[0]){
		case '1':
			TcpServer_Mode();
			break;
		case '2':
			TcpClient_Mode();
			break;
		case '3':
			UdpServer_Mode();
			break;
		case '4':
			UdpClient_Mode();
			break;
		default:
			log_msg("mode:%s",mode);
			break;
	}
	
	log_msg("\n********%s done!********\n",__FUNCTION__);
	return 0;
}

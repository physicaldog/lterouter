#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h>  
#include "./uci.h" 
#define BUFF_LEN 64
int get_config_cmd(char *package,char *section,char *option,char *buff) 
{ 
	FILE *fp = NULL;
	char cmd[64] = {0};
	//char buff[64] = {0};

	sprintf(cmd,"uci -c /opt/config get %s.%s.%s",package,section,option); 

	fp = popen(cmd,"r");
	if(fp){
		fread(buff,1,BUFF_LEN,fp);
		fclose(fp);
	}
	//printf("system_time=%s\n",system_time);

	return(0); 
}

int set_config_cmd(char *package,char *section,char *option,char *value,bool commit) 
{ 
	FILE *fp = NULL;
	char cmd[64] = {0};
	char rbuff[64] = {0};

	sprintf(cmd,"uci -c /opt/config set %s.%s.%s=%s",package,section,option,value); 

	fp = popen(cmd,"r");
	if(fp){
		fread(rbuff,1,sizeof(rbuff),fp);
		fclose(fp);
	}
	if(commit){
		memset(cmd,0,sizeof(cmd));
		sprintf(cmd,"uci -c /opt/config commit %s.%s.%s",package,section,option); 
		fp = popen(cmd,"r");
		if(fp){
			//fread(buff,1,sizeof(buff),fp);
			fclose(fp);
		}
	}

	return(0); 
}
int main(int argc,char *argv[])
{
	char buff[BUFF_LEN] = {0};
	if(argc < 5){
		printf("no argc!!!\n");
		return -1;
	}

	if(!strcmp(argv[1],"get")){
		get_config_cmd(argv[2],argv[3],argv[4],buff);
		printf("%s",buff);
	}
	if(!strcmp(argv[1],"set")){
		if(argc < 7){
			printf("no argc!!!\n");
			return -1;
		}
		if(!strcmp(argv[6],"commit"))
			set_config_cmd(argv[2],argv[3],argv[4],argv[5],true);
		else
			set_config_cmd(argv[2],argv[3],argv[4],argv[5],false);
	}
}

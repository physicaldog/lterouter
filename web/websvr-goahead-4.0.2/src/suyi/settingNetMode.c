#include"suyi_common.h"
#define LanConf "/opt/config/LanConfig"


void checkNetMode(Webs *wp)
{
	int ret = 0;
	FILE *fp = NULL;
	char cmd[128] = {0};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	
	memset(cmd,0,sizeof(cmd));
	printf("1\n");
	fp = fopen("/opt/init/MASQUERADE.sh","r+");
	if(fp){
		fseek(fp, 0, SEEK_SET);
		ret = fread(cmd,sizeof(char),sizeof(cmd),fp);
		if(strstr(cmd,"MASQUERADE")){
			websWrite(wp,("2"));
		}
		else
			websWrite(wp,("1"));
		fclose(fp);
	}
	else{
		websWrite(wp,("读取网络模式失败"));
	}
		
	websDone(wp);
	return;
}
void set_mode_1(Webs *wp)
{
	char *lanip;
	char *netmask;
	int ret = 0;
	FILE *fp = NULL;
	char cmd[16] = "#!/bin/bash\n";
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	
	fp = fopen("/opt/init/MASQUERADE.sh","w+");
	if(fp){
		ret = fwrite(cmd,sizeof(char),strlen(cmd),fp);
		printf("fopen OK!,%d,%d\n",ret,strlen(cmd));
		if(ret == strlen(cmd)){
			websWrite(wp,("重启后生效"));
		}
		else
			websWrite(wp,("操作失败"));
	}
	else
		websWrite(wp,("操作失败"));
	fclose(fp);
		
	websDone(wp);
	return;
}

void set_mode_2(Webs *wp)
{
	int ret = 0;
	FILE *fp = NULL;
	char cmd[128] = "#!/bin/bash\niptables -t nat -A POSTROUTING -o usb0 -j MASQUERADE";
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	fp = fopen("/opt/init/MASQUERADE.sh","w+");
	if(fp){
		if(strlen(cmd) == fwrite(cmd,1,strlen(cmd),fp)){
			websWrite(wp,("重启后生效"));
		}
		else
			websWrite(wp,("操作失败"));
	}
	else
		websWrite(wp,("操作失败"));

	fclose(fp);
	websDone(wp);
	return;
}

void set_snat(Webs *wp)
{
	
	int ret = 0;
	FILE *fp = NULL;
	char *source_ip;
	char *source_port;
	char *snat_ip;
	char *snat_port;
	char *snat_opt;
	char cmd[128] = {0};
	char opt[32] = {0};
	char sport[32] = {0};
	char dport[32] = {0};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	source_ip = websGetVar(wp,("source_ip"),(""));
	source_port = websGetVar(wp,("source_port"),(""));
	snat_ip = websGetVar(wp,("snat_ip"),(""));
	snat_port = websGetVar(wp,("snat_port"),(""));
	snat_opt = websGetVar(wp,("snat_opt"),(""));
	printf("%s,%s,%s\n",source_port,snat_port,snat_opt);
	if(*snat_opt != '1')
		sprintf(opt,"-p %s",snat_opt);
	if(*source_port != '\0')
		sprintf(sport,"--sport %s",source_port);
	if(*snat_port != '\0')
		sprintf(dport,":%s",snat_port);


	sprintf(cmd,"iptables -t nat -A POSTROUTING -o usb0 %s -s %s %s -j SNAT --to %s%s\n",opt,source_ip,sport,snat_ip,dport);

	fp = fopen("/opt/init/nat.sh","a+");
	if(fp){
		if((strlen(cmd) == fwrite(cmd,1,strlen(cmd),fp))){
			websWrite(wp,("重启后生效"));
		}
		else
			websWrite(wp,("操作失败"));
	}
	else
		websWrite(wp,("操作失败"));

	fclose(fp);
	websDone(wp);
	return;
}
void set_dnat(Webs *wp)
{
	
	int ret = 0;
	FILE *fp = NULL;
	char *destination_ip;
	char *destination_port;
	char *dnat_ip;
	char *dnat_port;
	char *dnat_opt;
	char cmd[128] = {0};
	char opt[32] = {0};
	char sport[32] = {0};
	char dport[32] = {0};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	destination_ip = websGetVar(wp,("destination_ip"),(""));
	destination_port = websGetVar(wp,("destination_port"),(""));
	dnat_ip = websGetVar(wp,("dnat_ip"),(""));
	dnat_port = websGetVar(wp,("dnat_port"),(""));
	dnat_opt = websGetVar(wp,("dnat_opt"),(""));
	printf("%s,%s,%s\n",destination_port,dnat_port,dnat_opt);
	if(*dnat_opt != '1')
		sprintf(opt,"-p %s",dnat_opt);
	if(*destination_port!= '\0')
		sprintf(sport,"--dport %s",destination_port);
	if(*dnat_port != '\0')
		sprintf(dport,":%s",dnat_port);
	sprintf(cmd,"iptables -t nat -A PREROUTING -i usb0 %s -d %s %s -j DNAT --to %s%s\n",opt,destination_ip,sport,dnat_ip,dport);

	fp = fopen("/opt/init/nat.sh","a+");
	if(fp){
		if((strlen(cmd) == fwrite(cmd,1,strlen(cmd),fp))){
			websWrite(wp,("重启后生效"));
		}
		else
			websWrite(wp,("操作失败"));
	}
	else
		websWrite(wp,("操作失败"));

	fclose(fp);
	websDone(wp);
	return;
}
void clear_nat(Webs *wp)
{
	int ret = 0;
	FILE *fp = NULL;
	char cmd[16] = "#!/bin/bash\n";
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	fp = fopen("/opt/init/nat.sh","w+");
	if(fp){
		if((strlen(cmd) == fwrite(cmd,1,strlen(cmd),fp))){
			websWrite(wp,("重启后生效"));
		}
		else
			websWrite(wp,("操作失败"));
	}
	else
		websWrite(wp,("操作失败"));

	fclose(fp);
	websDone(wp);
	return;
}
void query_nat(Webs *wp)
{
	FILE * fp;
	int ret =0;
	char rbuff[512];
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);

	popen("iptables -L -t nat > /opt/log/iptables","r");
	sleep(4);
	fp = fopen("/opt/log/iptables","r");
    if (NULL == fp){
            printf("no iptables log\n");
            return -1;
    }

	while(1){

		ret = fread(rbuff, 1, sizeof(rbuff)-1, fp);
		printf("read ret = %d\n",ret);
		websWrite(wp,("%s"),rbuff);
		if((sizeof(rbuff) - 1) > ret){
			printf("read end\n");
			break;
		}
		memset(rbuff,0,sizeof(rbuff));
	}
	fclose(fp);
	 
	websDone(wp);
	return;
}

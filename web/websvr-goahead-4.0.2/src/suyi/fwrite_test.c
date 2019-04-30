#include<stdlib.h>
#include<string.h>
#include<stdio.h>
int main()
{
	FILE *fp = NULL;
	char cmd[16] = "bash\n";
	printf("\n********%s********\n",__FUNCTION__);
	fp = fopen("/opt/init/nat_1.sh","w+");
	if(fp){
		if(strlen(cmd) == fwrite(cmd,sizeof(char),strlen(cmd),fp)){
			printf("重启后生效");
		}
		else
			printf("重启后生效");
	}
	else
		printf("重启后生效");

	return 0;
}

#include"suyi_common.h"
#define UserPW "/opt/config/UserPW"

void loginCheck(webs_t wp, char_t *path, char_t *query)
{
	char *username;
	char *password;
	char name[16] = {'\0'};
	char PW[16] = {'\0'};

	printf("loginCheck!\n");
	username = websGetVar(wp,T("username"),T(""));
	password = websGetVar(wp,T("password"),T(""));
	strcat(name,username);
	//strcat(name,":");
	getConfig(name,PW,UserPW);
	printf("uname:%s,uPW:%s,PW:%s\n",username,password,PW);
	if(0 == strcmp(PW,password))
	{
		printf("验证成功\n");
		websWrite(wp,T("1"));
	}
	else
	{
		printf("验证失败\n");
		websWrite(wp,T("0"));
	}


	websDone(wp,200);
	/*系统重启*/
	return;
}

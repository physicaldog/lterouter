#include"suyi_common.h"
#define UserPW "/opt/config/UserPW"

void loginCheck(Webs *wp)
{
	char *username;
	char *password;
	char name[16] = {'\0'};
	char PW[16] = {'\0'};

	printf("\n********%s********!\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	username = websGetVar(wp,("username"),(""));
	password = websGetVar(wp,("password"),(""));
	strcat(name,username);
	//strcat(name,":");
	//getConfig(name,PW,UserPW);
	printf("uname:%s,uPW:%s,PW:%s\n",username,password,PW);
	if(0 == strcmp(PW,password))
	{
		printf("验证成功\n");
		websWrite(wp,("1"));
	}
	else
	{
		printf("验证失败\n");
		websWrite(wp,("0"));
	}


	websDone(wp);
	/*系统重启*/
	return;
}
void logincheck(Webs *wp)
{
	char *username;
	char *password;
	char name[16] = {'\0'};
	char PW[16] = {'\0'};

	printf("loginCheck!\n");
	username = websGetVar(wp,"username","");
	password = websGetVar(wp,"password","");
	strcat(name,username);
	printf("%s,%s\n",username,password);
	//strcat(name,":");
	//getConfig(name,PW,UserPW);
	printf("uname:%s,uPW:%s,PW:%s\n",username,password,PW);
	if(0 == strcmp(PW,password))
	{
		printf("验证成功\n");
		websWrite(wp,"111111");
	}
	else
	{
		printf("验证失败\n");
		websWrite(wp,"0");
	}
	websWriteBlock(wp,"monkey", 6);
	websFlush(wp, 0);
	websRedirectByStatus(wp, HTTP_CODE_OK);
	websDone(wp);
	/*系统重启*/
	return;
}

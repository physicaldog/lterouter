#include"suyi_common.h"
#define UserPW "/opt/config/UserPW"

void settingPW(webs_t wp, char_t *path, char_t *query)
{
	char *currPW;
	char *newPW;
	char PW[16] = {'\0'};
	printf("settingPW\n");
	currPW = websGetVar(wp,T("currPW"),T(""));
	newPW = websGetVar(wp,T("newPW"),T(""));
	printf("currPW:%s,PW:%s\n",currPW,newPW);
	getConfig("suyi:",PW,UserPW);
	//printf("PW:%s\n",PW);
	if((0 == strcmp(PW,currPW))){
		setConfig("suyi:",newPW,UserPW);
		websWrite(wp,T("设置成功"));
	}
	else{
		websWrite(wp,T("密码错误"));
	}
	websDone(wp,200);
	return;
}

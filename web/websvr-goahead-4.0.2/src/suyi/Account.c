#include"suyi_common.h"
#define UserPW "/opt/config/UserPW"

void settingPW(Webs *wp)
{
	char *currPW;
	char *newPW;
	char PW[16] = {'\0'};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	currPW = websGetVar(wp,("currPW"),(""));
	newPW = websGetVar(wp,("newPW"),(""));
	printf("currPW:%s,PW:%s\n",currPW,newPW);
	//getConfig("suyi",PW,UserPW);
	//printf("PW:%s\n",PW);
	if((0 == strcmp(PW,currPW))){
		//setConfig("suyi",newPW,UserPW);
		websWrite(wp,("设置成功"));
	}
	else{
		websWrite(wp,("密码错误"));
	}
	websDone(wp);
	return;
}

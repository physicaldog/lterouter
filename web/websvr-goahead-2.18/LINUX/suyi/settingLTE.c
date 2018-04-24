#include"suyi_common.h"

void settingAPN(webs_t wp, char_t *path, char_t *query)
{
	char *apn;

	apn = websGetVar(wp,T("apn"),T("jk"));
	printf("settingAPN apn=%s\n",apn);

	setConfig("apn:",apn);

    websWrite(wp,T("重启后生效"));
	websDone(wp,200);
	return;
}

void queryAPN(webs_t wp, char_t *path, char_t *query)
{
        char buff[64] = {'\0'};//读取文件缓存

        printf("apn query\n");
        getConfig("apn:",buff);
        printf("apn = %s\n", buff);

        websWrite(wp,T("%s"),buff);

        websDone(wp,200);
        return;
}

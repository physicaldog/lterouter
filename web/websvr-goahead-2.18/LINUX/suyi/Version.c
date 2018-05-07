#include"suyi_common.h"
#define Version "/opt/config/Version"

void queryVer(webs_t wp, char_t *path, char_t *query)
{
    char pName[32] = {'\0'};
    char pModle[32] = {'\0'};
    char sVer[32] = {'\0'};
    printf("version query\n");
    getConfig("productName:",pName,Version);
    getConfig("productModle:",pModle,Version);
    getConfig("softVersion:",sVer,Version);
    printf("pName:%s,pModle:%s,sVer:%s\n",pName,pModle,sVer);

    websWrite(wp,T("{"));
    websWrite(wp,T("\"pName\":\"%s\","),pName);
    websWrite(wp,T("\"pModle\":\"%s\","),pModle);
    websWrite(wp,T("\"sVer\":\"%s\""),sVer);
    websWrite(wp,T("}"));

    websDone(wp,200);
	return;
}

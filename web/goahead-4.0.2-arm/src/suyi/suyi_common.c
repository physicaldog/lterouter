#include    "suyi_common.h"
void testFun(Webs *wp)
{
	printf("%s\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
	//websWrite(wp, "Name %s", websGetVar(wp, "name", "")); 
	//websWrite(wp,  "Age %s", websGetVar(wp, "age", ""));
	websWrite(wp, "Name monkey\n"); 
	//websWrite(wp,  "Age 20");
	websDone(wp);

}

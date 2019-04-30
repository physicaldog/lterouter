#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
 #include <fcntl.h>
#include "cJSON.h"
// 被解析的JSON数据包
char text[] = "{\"timestamp\":\"2013-11-19T08:50:11\",\"value\":1}";

void printJson(cJSON * root)//以递归的方式打印json的最内层键值对
{
	for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
	{
		cJSON * item = cJSON_GetArrayItem(root, i);        
		if(cJSON_Object == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
			printJson(item);
		else                                //值不为json对象就直接打印出键和值
		{
			printf("%s->", item->string);
			printf("%s\n", cJSON_Print(item));
		}
	}
}

int main (int argc, const char * argv[])
{
	cJSON *json , *json_value , *json_timestamp;
	FILE *fp = NULL;
	char buff[1024] = {0};
	char *ptr = NULL;
	int ret = 0;

	fp = fopen("./json.txt","r+");
	if(NULL == fp){
		printf("fopen failed!\n");
		return -1;
	}
	
	ret = fread(buff,1,sizeof(buff),fp);
	printf("ret = %d,%s,%d\n",ret,buff,strlen(buff));
	fclose(fp);

	// 解析数据包
	json = cJSON_CreateObject();
	if (!json)
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	else
	{
		cJSON_AddItemToObject(json,"name",cJSON_CreateString("suyi"));
		for(ret=0;ret<10000;ret++)
			cJSON_AddItemToObject(json,"addr",cJSON_CreateObject());
		while(1);

		fp = fopen("./json.txt","w+");
		if(NULL == fp){
			printf("fopen failed!\n");
			return -1;
		}
		ptr = cJSON_Print(json);
		printf("%s\n",ptr);
		ret = fwrite(ptr,1,strlen(ptr),fp);
		fclose(fp);

		free(ptr);
		cJSON_Delete(json);
	}
	return 0;
}

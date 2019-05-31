#include"suyi_common.h"
#define Version "/opt/config/Version"

void queryVer(Webs *wp)
{
    char pName[32] = {'\0'};
    char pModle[32] = {'\0'};
    char sVer[32] = {'\0'};
	printf("\n********%s********\n",__FUNCTION__);
	websSetStatus(wp, 200);
	websWriteHeaders(wp, -1, 0);//参数二需要未-1,否则前端收不到数据
	websWriteEndHeaders(wp);
    //getConfig("productName",pName,Version);
    //getConfig("productModle",pModle,Version);
    //getConfig("softVersion",sVer,Version);
    get_config(pName,"productName","system","config");
    get_config(pModle,"productModle","system","config");
    get_config(sVer,"softVersion","system","config");
    printf("pName:%s,pModle:%s,sVer:%s\n",pName,pModle,sVer);

    websWrite(wp,("{"));
    websWrite(wp,("\"pName\":\"%s\","),pName);
    websWrite(wp,("\"pModle\":\"%s\","),pModle);
    websWrite(wp,("\"sVer\":\"%s\""),sVer);
    websWrite(wp,("}"));

    websDone(wp);
	return;
}

int checkUploadFile()
{
    FILE *fe = NULL;
    char md5[64] = {'\0'};
	printf("\n********%s********\n",__FUNCTION__);

	system("cd /tmp;dd if=system.tar |openssl des3 -d -k monkey|tar xf -; cd ./system/;md5sum -c cpe.md5 > md5.sum");
	
	fe = fopen("/tmp/system/md5.sum","r+");
	if (NULL == fe){
		printf("no md5sum file\n");
		system("rm /tmp/md5.sum");
		return -1;//未找到配置文件
	}

	fgets(md5,sizeof(md5),fe);
	if (strstr(md5,"OK")){
		printf("Check OK!\n");
		system("mv /tmp/system/cpe.tar /opt/;rm -r /tmp/system");
	}
	else{
		printf("Check failed!\n");
		system("rm -r /tmp/system");
		fclose(fe);
		return -1;
	}

	fclose(fe);
	return 0;
}

/*
void upGrade(Webs *wp) 
{
    FILE *fp;
    char_t *fn;
    char_t *bn = NULL;
    int locWrite;
    int numLeft;
    int numWrite;
    int ret = 0;;
	char Path[32] = {'\0'};

    a_assert(websValid(wp));
    websHeader(wp);

	printf("********upGrade********\n");
    fn = websGetVar(wp, ("filename"), (""));
    if (fn != NULL && *fn != '\0') {
        if ((int)(bn = gstrrchr(fn, '/') + 1) == 1) {
            if ((int)(bn = gstrrchr(fn, '\\') + 1) == 1) {
                bn = fn;
            }
        }
    }

    //websWrite(wp, ("升级包： = %s<br>升级包大小： = %d bytes<br>"), bn, wp->lenPostData);
    websWrite(wp, ("文件名： %s<br><br>"), bn);
	printf("fn:%s,bn:%s\n",fn,bn);
	strcat(Path,"/tmp/");
	strcat(Path,bn);
    //if ((fp = fopen((bn == NULL ? "upldForm.bin" : bn), "w+b")) == NULL) {
    if ((fp = fopen(Path, "w+b")) == NULL) {
        websWrite(wp, ("File open failed!<br>"));
    } else {
        locWrite = 0;
        numLeft = wp->lenPostData;
        while (numLeft > 0) {
            numWrite = fwrite(&(wp->postData[locWrite]), sizeof(*(wp->postData)), numLeft, fp);
            if (numWrite < numLeft) {
                websWrite(wp, ("File write failed.<br>  ferror=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), ferror(fp), locWrite, numLeft, numWrite, wp->lenPostData);
            break;
            }
            locWrite += numWrite;
            numLeft -= numWrite;
        }

        if (numLeft == 0) {
            if (fclose(fp) != 0) {
                websWrite(wp, ("File close failed.<br>  errno=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), errno, locWrite, numLeft, numWrite, wp->lenPostData);
            } else {
				ret = checkMD5();	
				if(0 == ret)
					websWrite(wp, ("文件大小： %d bytes<br><br>校验成功，重启设备以完成升级！"), wp->lenPostData);
				else
					websWrite(wp, ("文件大小： %d bytes<br><br>校验失败，请重新上传文件！"), wp->lenPostData);
					
            }
        } else {
            websWrite(wp, ("numLeft=%d locWrite=%d Size=%d bytes<br>"), numLeft, locWrite, wp->lenPostData);
        }
    }

    websFooter(wp);
    websDone(wp, 200);
} */   /* void upldForm(Webs *wp wp, char_t * path, char_t * query) */

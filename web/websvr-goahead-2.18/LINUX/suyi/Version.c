#include"suyi_common.h"
#define Version "/opt/config/Version"
#define MD5 "/opt/md5.sum"

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

int checkMD5()
{
    FILE *fe = NULL;
    char md5[64] = {'\0'};

	printf("********checkfile********\n");
	system("cd /opt/;tar xvf system.tar;md5sum -c cpe.md5 > md5.sum");
	
	fe = fopen(MD5,"r+");
	if (NULL == fe){
		printf("no md5sum file\n");
		return -1;//未找到配置文件
	}

	fgets(md5,sizeof(md5),fe);
	if (strstr(md5,"OK")){
		printf("Check OK!\n");
		system("cd /opt;rm *.md5 *.sum system.tar");
	}
	else{
		printf("Check failed!\n");
		system("cd /opt;rm *.md5 *.sum *.tar");
		fclose(fe);
		return -1;
	}

	fclose(fe);
	return 0;
}


void upGrade(webs_t wp, char_t * path, char_t * query) 
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
    fn = websGetVar(wp, T("filename"), T(""));
    if (fn != NULL && *fn != '\0') {
        if ((int)(bn = gstrrchr(fn, '/') + 1) == 1) {
            if ((int)(bn = gstrrchr(fn, '\\') + 1) == 1) {
                bn = fn;
            }
        }
    }

    //websWrite(wp, T("升级包： = %s<br>升级包大小： = %d bytes<br>"), bn, wp->lenPostData);
    websWrite(wp, T("文件名： %s<br><br>"), bn);
	printf("fn:%s,bn:%s\n",fn,bn);
	strcat(Path,"/opt/");
	strcat(Path,bn);
    //if ((fp = fopen((bn == NULL ? "upldForm.bin" : bn), "w+b")) == NULL) {
    if ((fp = fopen(Path, "w+b")) == NULL) {
        websWrite(wp, T("File open failed!<br>"));
    } else {
        locWrite = 0;
        numLeft = wp->lenPostData;
        while (numLeft > 0) {
            numWrite = fwrite(&(wp->postData[locWrite]), sizeof(*(wp->postData)), numLeft, fp);
            if (numWrite < numLeft) {
                websWrite(wp, T("File write failed.<br>  ferror=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), ferror(fp), locWrite, numLeft, numWrite, wp->lenPostData);
            break;
            }
            locWrite += numWrite;
            numLeft -= numWrite;
        }

        if (numLeft == 0) {
            if (fclose(fp) != 0) {
                websWrite(wp, T("File close failed.<br>  errno=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes<br>"), errno, locWrite, numLeft, numWrite, wp->lenPostData);
            } else {
				ret = checkMD5();	
				if(0 == ret)
					websWrite(wp, T("文件大小： %d bytes<br><br>校验成功，重启设备以完成升级！"), wp->lenPostData);
				else
					websWrite(wp, T("文件大小： %d bytes<br><br>校验失败，请重新上传文件！"), wp->lenPostData);
					
            }
        } else {
            websWrite(wp, T("numLeft=%d locWrite=%d Size=%d bytes<br>"), numLeft, locWrite, wp->lenPostData);
        }
    }

    websFooter(wp);
    websDone(wp, 200);
}    /* void upldForm(webs_t wp, char_t * path, char_t * query) */

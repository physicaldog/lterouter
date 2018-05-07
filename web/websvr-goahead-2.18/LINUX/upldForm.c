/* upldForm.c - GoForm to handle file upload */

/*
modification history
--------------------
01a,13feb01,lohct  written.
*/

/*
DESCRIPTION
This GoForm procedure handles http file upload.

SEE ALSO:
"GoForms - GoAhead WebServer"
*/

#include "../webs.h"

/* forward declarations */
void        upldForm(webs_t wp, char_t * path, char_t * query);


/*******************************************************************************
*
* upldForm - GoForm procedure to handle file uploads for upgrading device
*
* This routine handles http file uploads for upgrading device.
*
* RETURNS: OK, or ERROR if the I/O system cannot install the driver.
*/

void upldForm(webs_t wp, char_t * path, char_t * query) {
    FILE *       fp;
    char_t *     fn;
    char_t *     bn = NULL;
    int          locWrite;
    int          numLeft;
    int          numWrite;
	char Path[32] = {'\0'};

    a_assert(websValid(wp));
    websHeader(wp);

    fn = websGetVar(wp, T("filename"), T(""));
    if (fn != NULL && *fn != '\0') {
        if ((int)(bn = gstrrchr(fn, '/') + 1) == 1) {
            if ((int)(bn = gstrrchr(fn, '\\') + 1) == 1) {
                bn = fn;
            }
        }
    }

    websWrite(wp, T("Filename = %s<br>Size = %d bytes<br>"), bn, wp->lenPostData);
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
                websWrite(wp, T("File Size Written = %d bytes<br>"), wp->lenPostData);
            }
        } else {
            websWrite(wp, T("numLeft=%d locWrite=%d Size=%d bytes<br>"), numLeft, locWrite, wp->lenPostData);
        }
    }

    websFooter(wp);
    websDone(wp, 200);
}    /* void upldForm(webs_t wp, char_t * path, char_t * query) */

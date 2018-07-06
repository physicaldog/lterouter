/*
 * main.c -- Main program for the GoAhead WebServer (LINUX version)
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: main.c,v 1.5 2003/09/11 14:03:46 bporter Exp $
 */

/******************************** Description *********************************/

/*
 *	Main program for for the GoAhead WebServer. This is a demonstration
 *	main program to initialize and configure the web server.
 */

/********************************* Includes ***********************************/

#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include <syslog.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif

#include <fcntl.h> /* for open/close */
#include <termios.h> /* for serial port */

#include "a6g2cimpl.h"
//#include "router_status.h"
//#include "router_set.h"
#include "suyi/suyi_common.h"

/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */

static char_t		*rootWeb = T("/opt/web/staticPage");	/* Root web directory */
static char_t		*password = T("");				/* Security password */
static int			port = 80;						/* Server port */
static int			retries = 5;					/* Server port retries */
static int			finished;						/* Finished flag */

/****************************** Forward Declarations **************************/

static int 	initWebs();

#if 1
static int	query(int eid, webs_t wp, int argc, char_t **argv);
static void set(webs_t wp, char_t *path, char_t *query);

static int	aspTest(int eid, webs_t wp, int argc, char_t **argv);
static void formTest(webs_t wp, char_t *path, char_t *query);
#endif

static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t *url, char_t *path, char_t *query);
extern void defaultErrorHandler(int etype, char_t *msg);
extern void defaultTraceHandler(int level, char_t *buf);
extern void upldForm(webs_t wp, char_t *path, char_t *query);
#ifdef B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif

/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */

int main(int argc, char** argv)
{
/*
 *	Initialize the memory allocator. Allow use of malloc and start
 *	with a 60K heap.  For each page request approx 8KB is allocated.
 *	60KB allows for several concurrent page requests.  If more space
 *	is required, malloc will be used for the overflow.
 */
	bopen(NULL, (60 * 1024), B_USE_MALLOC);
	signal(SIGPIPE, SIG_IGN);

/*
 *	Initialize the web server
 */
	if (initWebs() < 0) {
		return -1;
	}

#ifdef WEBS_SSL_SUPPORT
	websSSLOpen();
#endif

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */
	while (!finished) {
		if (socketReady(-1) || socketSelect(-1, 1000)) {
			socketProcess(-1);
		}
		websCgiCleanup();
		emfSchedProcess();
	}

#ifdef WEBS_SSL_SUPPORT
	websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
	umClose();
#endif

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
	websCloseServer();
	socketClose();
#ifdef B_STATS
	memLeaks();
#endif
	bclose();
	return 0;
}

/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs()
{
	struct hostent	*hp;
	struct in_addr	intaddr;
	char			host[128], dir[128], webdir[128];
	char			*cp;
	char_t			wbuf[128];

	hp = NULL;
	dir[0] = '\0';
/*
 *	Initialize the socket subsystem
 */
	socketOpen();

#ifdef USER_MANAGEMENT_SUPPORT
/*
 *	Initialize the User Management database
 */
	umOpen();
	umRestore(T("umconfig.txt"));
    //printf("open um!\n");   //for debug by xie
#endif

#if 0
/*
 *	Define the local Ip address, host name, default home page and the
 *	root web directory.
 */
	if (gethostname(host, sizeof(host)) < 0) {
		error(E_L, E_LOG, T("Can't get hostname"));
		return -1;
	}

    printf("Debug the hosts is: %s\n", host);   //for debug by xie

	if ((hp = gethostbyname(host)) == NULL) {
        printf("Can't get host address\n");   //for debug by xie
		error(E_L, E_LOG, T("Can't get host address"));
		return -1;
	}

	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
		(size_t) hp->h_length);

/*
 *	Set ../web as the root web. Modify this to suit your needs
 */
 	getcwd(dir, sizeof(dir));
 	if ((cp = strrchr(dir, '/'))) {
 		*cp = '\0';
 	}
    sprintf(webdir, "%s/%s", dir, rootWeb);
#endif
    sprintf(webdir, "%s", rootWeb);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultDir(webdir);
	cp = inet_ntoa(intaddr);
	ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
	websSetIpaddr(wbuf);
	ascToUni(wbuf, host, min(strlen(host) + 1, sizeof(wbuf)));
	websSetHost(wbuf);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultPage(T("default.asp"));
	websSetPassword(password);

/*
 *	Open the web server on the given port. If that port is taken, try
 *	the next sequential port for up to "retries" attempts.
 */
	websOpenServer(port, retries);

/*
 * 	First create the URL handlers. Note: handlers are called in sorted order
 *	with the longest path handler examined first. Here we define the security
 *	handler, forms handler and the default web page handler.
 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler,
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler,
		WEBS_HANDLER_LAST);
/*
 *	roter-manager handler function
 *	
 */
/*添加系统日志*/
openlog("router_web", LOG_CONS | LOG_PID, LOG_LOCAL2);
/*security*/
	websFormDefine(T("startSecurity"), startSecurity);
	websFormDefine(T("cancelSecurity"), cancelSecurity);
	websFormDefine(T("settingSecurity"), settingSecurity);
	websFormDefine(T("querySecurity"), querySecurity);
	websFormDefine(T("getSecurityLog"), getSecurityLog);

/*Diagnose*/
	websFormDefine(T("cleanLog"), cleanLog);
	websFormDefine(T("getPingLog"), getPingLog);
	websFormDefine(T("pingTest"), pingTest);
/*Version*/
	websFormDefine(T("queryVer"), queryVer);
	websFormDefine(T("upldForm"), upGrade);
	//websFormDefine(T("upldForm"), upldForm);
/*login*/
	websFormDefine(T("loginCheck"), loginCheck);
/*Account*/
	websFormDefine(T("settingPW"), settingPW);
/*settingWH*/
	websFormDefine(T("queryTime"), queryTime);
	websFormDefine(T("settingTime"), settingTime);
	websFormDefine(T("resetDev"), resetDev);
	websFormDefine(T("sysReset"), sysReset);
/*settingLAN*/
	websFormDefine(T("settingLAN"), settingLAN);
	websFormDefine(T("queryLAN"), queryLAN);

/*seetingLTE*/
	websFormDefine(T("queryIMSI"), queryIMSI);
	websFormDefine(T("settingAPN"), settingAPN);
	websFormDefine(T("queryAPN"), queryAPN);
/*settingDtu*/
	websFormDefine(T("startDtu"), startDtu);
	websFormDefine(T("cancelDtu"), cancelDtu);
	websFormDefine(T("settingDtu"), settingDtu);
	websFormDefine(T("queryDtu"), queryDtu);
/*Runningstatus*/
	websFormDefine(T("sysInfo"), sysInfo);
	websFormDefine(T("deviceInfo"), deviceInfo);
	websFormDefine(T("WANStatus"), WANStatus);
	websFormDefine(T("LANStatus"), LANStatus);
//	
//	websFormDefine(T("lteSet_apn"), apn_set);
//	websFormDefine(T("lteSet_lanip"), lanip_set);
//	websFormDefine(T("lteStatus_lanip"), lanip_query);
//    
//    
//	websFormDefine(T("sysSet_sysReset"), sysReset);
//	websFormDefine(T("sysSet_moduleReset"), moduleReset);

/*
 *	Now define the procedures. Replace these with your application
 *	relevant ASP script procedures and form functions.
 */
	websFormDefine(T("gpio_p34"), on_gp34_set);
	
	websFormDefine(T("led"), on_led_set);
	websFormDefine(T("buzzer"), on_buzzer_set);

	websFormDefine(T("lcdbkl"), on_bkl_set);	
	

//	websAspDefine(T("query"), query);
//	websFormDefine(T("set"), set);
	websAspDefine(T("aspTest"), aspTest);
	websFormDefine(T("formTest"), formTest);

/*
 *	Create the Form handlers for the User Management pages
 */
#ifdef USER_MANAGEMENT_SUPPORT
	formDefineUserMgmt();
#endif

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0);
	return 0;
}

#if 1
/******************************************************************************/
/*
 *	Test Javascript binding for ASP. This will be invoked when "aspTest" is
 *	embedded in an ASP page. See web/asp.asp for usage. Set browser to
 *	"localhost/asp.asp" to test.
 */

static int query(int eid, webs_t wp, int argc, char_t **argv)
{
    char_t *wan_ip="192.168.1.1", *lan_ip="192.168.1.136", *apn="cmnet";
    printf("query\n");
    websWrite(wp, T("{\"wan_ip\":\"%s\",\"lan_ip\":\"%s\",\"apn\":\"%s\"}"), wan_ip, lan_ip, apn);
	return 0; 
}

static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*name, *address;
    //char_t *x = "123";
	if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
    
    //ejSetVar(eid,T("x"),x);

    printf("monkey\n");
	return websWrite(wp, T("{\"name\":\"%s\",\"addr\":\"%s\"}"), name, address);
}

/******************************************************************************/
/*
 *	Test form for posted data (in-memory CGI). This will be called when the
 *	form in web/forms.asp is invoked. Set browser to "localhost/forms.asp" to test.
 */
/*
static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t	*name, *address;

	name = websGetVar(wp, T("name"), T("Joe Smith"));
	address = websGetVar(wp, T("address"), T("1212 Milky Way Ave."));

	websHeader(wp);
	websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
	websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
	websFooter(wp);
	websDone(wp, 200);
}
*/
static void set(webs_t wp, char_t *path, char_t *query)
{
	char_t *wan_ip, *lan_ip, *apn, *subip_start, *subip_end, *submit;
    FILE *fd = NULL;
    int ret = 0;

    printf("set");

    if(strcmp("SET",submit) == 0)
    {
        wan_ip = websGetVar(wp, T("wan_ip"), T("no define"));
        lan_ip = websGetVar(wp, T("lan_ip"), T("no define"));
        apn = websGetVar(wp, T("apn"), T("no define"));
        subip_start = websGetVar(wp, T("subip_start"), T("no define"));
        subip_end = websGetVar(wp, T("subip_end"), T("no define"));
        printf("%s,%s,%s,%s\n",wan_ip,lan_ip,apn);

        fd = fopen("/opt/netconfig","r+");
        if(NULL == fd){
                perror("fopen failed!");
        }
        ret = fprintf(fd,"%s\n%s\n",wan_ip,lan_ip);
        printf("ret=%d\n",ret);
        fclose(fd);

        websHeader(wp);
        websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
        websWrite(wp, T("<body><h2>wan_ip: %s, lan_ip: %s</h2>\n"), wan_ip, lan_ip);
        websFooter(wp);
    }
    else if(strcmp("QUERY",submit) == 0)
    {
        websHeader(wp);
        websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
        websWrite(wp, T("<body><h2>look at this:</h2>\n"));
        websFooter(wp);

    }
    else
    {
        websHeader(wp);
        websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
        websWrite(wp, T("<body><h2>monkey:</h2>\n"));
        websFooter(wp);
        
    }

	websDone(wp, 200);
}

static void formTest(webs_t wp, char_t *path, char_t *query)
{
	char_t *wan_ip, *lan_ip, *apn, *subip_start, *subip_end, *submit;
    FILE *fd = NULL;
    int ret = 0;

    printf("star");
    if(strcmp("SET",submit) == 0)
    {
        wan_ip = websGetVar(wp, T("wan_ip"), T("no define"));
        lan_ip = websGetVar(wp, T("lan_ip"), T("no define"));
        apn = websGetVar(wp, T("apn"), T("no define"));
        subip_start = websGetVar(wp, T("subip_start"), T("no define"));
        subip_end = websGetVar(wp, T("subip_end"), T("no define"));
        printf("%s,%s,%s,%s\n",wan_ip,lan_ip,apn);

        fd = fopen("/opt/netconfig","r+");
        if(NULL == fd){
                perror("fopen failed!");
        }
        ret = fprintf(fd,"%s\n%s\n",wan_ip,lan_ip);
        printf("ret=%d\n",ret);
        fclose(fd);

        websHeader(wp);
        websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
        websWrite(wp, T("<body><h2>wan_ip: %s, lan_ip: %s</h2>\n"), wan_ip, lan_ip);
        websFooter(wp);
    }
    else if(strcmp("QUERY",submit) == 0)
    {
        websHeader(wp);
        websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
        websWrite(wp, T("<body><h2>look at this:</h2>\n"));
        websFooter(wp);

    }
    else
    {
        websHeader(wp);
        websWrite(wp, T("<header><title>A6G2C Web Application</title>\n<link rel=\"stylesheet\" href=\"../style/normal_ws.css\" type=\"text/css\" /></header>\n"));
        websWrite(wp, T("<body><h2>monkey:</h2>\n"));
        websFooter(wp);
        
    }

	websDone(wp, 200);
}

#endif

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
		websRedirect(wp, T("login.html"));
		return 1;
	}
	return 0;
}

/******************************************************************************/
/*
 *	Default error handler.  The developer should insert code to handle
 *	error messages in the desired manner.
 */

void defaultErrorHandler(int etype, char_t *msg)
{
#if 0
	write(1, msg, gstrlen(msg));
#endif
}

/******************************************************************************/
/*
 *	Trace log. Customize this function to log trace output
 */

void defaultTraceHandler(int level, char_t *buf)
{
/*
 *	The following code would write all trace regardless of level
 *	to stdout.
 */
#if 0
	if (buf) {
		write(1, buf, gstrlen(buf));
	}
#endif
}

/******************************************************************************/
/*
 *	Returns a pointer to an allocated qualified unique temporary file name.
 *	This filename must eventually be deleted with bfree();
 */

char_t *websGetCgiCommName()
{
	char_t	*pname1, *pname2;

	//pname1 = tempnam(NULL, T("cgi"));
	pname1 = (char_t*)mkstemp(T("cgi"));
	pname2 = bstrdup(B_L, pname1);
	free(pname1);
	return pname2;
}

/******************************************************************************/
/*
 *	Launch the CGI process and return a handle to it.
 */

int websLaunchCgiProc(char_t *cgiPath, char_t **argp, char_t **envp,
					  char_t *stdIn, char_t *stdOut)
{
	int	pid, fdin, fdout, hstdin, hstdout, rc;

	fdin = fdout = hstdin = hstdout = rc = -1;
	if ((fdin = open(stdIn, O_RDWR | O_CREAT, 0666)) < 0 ||
		(fdout = open(stdOut, O_RDWR | O_CREAT, 0666)) < 0 ||
		(hstdin = dup(0)) == -1 ||
		(hstdout = dup(1)) == -1 ||
		dup2(fdin, 0) == -1 ||
		dup2(fdout, 1) == -1) {
		goto DONE;
	}

 	rc = pid = fork();
 	if (pid == 0) {
/*
 *		if pid == 0, then we are in the child process
 */
		if (execve(cgiPath, argp, envp) == -1) {
			printf("content-type: text/html\n\n"
				"Execution of cgi process failed\n");
		}
		exit (0);
	}

DONE:
	if (hstdout >= 0) {
		dup2(hstdout, 1);
      close(hstdout);
	}
	if (hstdin >= 0) {
		dup2(hstdin, 0);
      close(hstdin);
	}
	if (fdout >= 0) {
		close(fdout);
	}
	if (fdin >= 0) {
		close(fdin);
	}
	return rc;
}

/******************************************************************************/
/*
 *	Check the CGI process.  Return 0 if it does not exist; non 0 if it does.
 */

int websCheckCgiProc(int handle)
{
/*
 *	Check to see if the CGI child process has terminated or not yet.
 */
	if (waitpid(handle, NULL, WNOHANG) == handle) {
		return 0;
	} else {
		return 1;
	}
}

/******************************************************************************/

#ifdef B_STATS
static void memLeaks()
{
	int		fd;

	if ((fd = gopen(T("leak.txt"), O_CREAT | O_TRUNC | O_WRONLY, 0666)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif


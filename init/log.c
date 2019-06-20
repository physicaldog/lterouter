#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>

//#define MAXLINE 256
#define MAXLINE 4096

static void log_doit(int, int, const char*, va_list ap);

#ifdef DEBUG
int debug = 1;
#else
int debug = 0;
#endif

void log_open(const char *ident, int option, int facility)
{
	printf("LOG_OPEN %d\n",debug);
	if(debug==0)
		openlog(ident, option, facility);
}

//打印应用信息和系统调用返回值，正常返回
void log_ret(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	return ;
}

//打印应用信息和系统调用返回值，程序退出
void log_quit(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

void log_msg(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	return ;
}

void log_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

static void log_doit(int errnoflag, int priority, const char *fmt, va_list ap)
{
	int errno_save;
	char buf[MAXLINE];

	errno_save = errno;
	vsprintf(buf, fmt, ap);
	if(errnoflag)
		sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	if(debug){
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	else
		syslog(priority, buf);
	return ;
}

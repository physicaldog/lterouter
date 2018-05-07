/************************************************************************
 * Id: log.c                                                            *
 *                                                                      *
 * TR069 Project:  A TR069 library in C                                 *
 * Copyright (C) 2013-2014  netcwmp.netcwmp group                                *
 *                                                                      *
 *                                                                      *
 * Email: netcwmp ( & ) gmail dot com                                *
 *                                                                      *
 * Modify: Andy_yang
 * QQ: 408797506 
 ***********************************************************************/

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdarg.h>

#include "cwmp/log.h"


#define MAX_LOGBUF_LENGTH   0x8000
#define MAX_LOGFILE_SIZE    0x80000

static long                 g_log_size;
static cwmp_log_t             g_log_context;

int cwmp_set_log_param(const char *path_name,  cwmp_log_level_t level, int enable_log, int enable_syslog, long size)
{
    int    ret = TRUE;
    char    *val = NULL;
    char    path[PATH_MAX] = {0};

    if(pthread_mutex_lock(&g_log_context.log_mutex) != 0)
    {
        return FALSE;
    }

    g_log_size = size;
    
    //取得level
    g_log_context.level = (level<CWMP_LOG_EMERG || level>CWMP_LOG_DEBUG)?DEFAULT_LOG_LEVEL:level;
    
    //取得log enable
    g_log_context.enable_log = (enable_log==TRUE || enable_log==FALSE)?enable_log: DEFAULT_LOG_ENABLE;;

    //取得syslog enable
    g_log_context.enable_log = (enable_syslog==TRUE || enable_syslog==FALSE)?enable_log: DEFAULT_LOG_ENABLE_SYS;;

    //取得路径
    if(g_log_context.fd != NULL && g_log_context.fd == stdout)       
    {
        fclose(g_log_context.fd);
        g_log_context.fd = NULL;
    }
    
     memset(g_log_context.fullname, 0, PATH_MAX);
     if(path_name)
     {
         snprintf(g_log_context.fullname, PATH_MAX, "%s", path_name);
     }
     else
     {
         snprintf(g_log_context.fullname, PATH_MAX, "%s", DEFAULT_LOG_PATHNAME);         
     }
     
     g_log_context.fd = fopen(g_log_context.fullname, "a+");
     if(g_log_context.fd == NULL)
     {
         ret = FALSE;
     }
    
    if(!path_name || ret == FALSE)
    {
        g_log_context.fd = stdout;

    }
    
finish:

    pthread_mutex_unlock(&g_log_context.log_mutex);
    return ret;
}

int init_log(const char *path_name, int enable_log, cwmp_log_level_t level,  int enable_syslog, long size)
{
    int            ret = FALSE;

    memset(&g_log_context, 0, sizeof(g_log_context));

    g_log_size = size;
    if(pthread_mutex_init(&g_log_context.log_mutex, NULL) != 0) 
    {
        return ret;
    }
    ret = cwmp_set_log_param(path_name, level, enable_log, enable_syslog, size);

    return ret;
}


static void log_write(const char * buf, size_t len)
{
    if(pthread_mutex_lock(&g_log_context.log_mutex) != 0)
    {
        return;
    }

    if(g_log_context.fd)
    {
        if(fwrite(buf, sizeof(char), len, g_log_context.fd) < 0 || fflush(g_log_context.fd) < 0)
        {
            fprintf(stderr, "log_write() write to log file failed! open fd again\n");
            if(g_log_context.fd > 0)
            {
                fclose(g_log_context.fd);
                fprintf(stderr, "log_write() write to log file failed! fd closed\n");
            }
            
            if((g_log_context.fd = fopen(g_log_context.fullname, "a+")) == NULL)
            {
                g_log_context.fd = 0;
            }
            else
            {
                if(fwrite(buf, sizeof(char), len, g_log_context.fd) < 0 || fflush(g_log_context.fd) < 0)
                    fprintf(stderr, "log_write() write to log file failed!\n");
            }
        }
    }

    pthread_mutex_unlock(&g_log_context.log_mutex);
}

static void log_checkfile(void)
{
    struct stat st;
    
    if(!g_log_context.fd)
    {
        return;
    }

    if(g_log_context.fd == stdout)
    {
        return;
    }
    
    if(pthread_mutex_lock(&g_log_context.log_mutex) != 0)
    {
        return;
    }

    if(stat(g_log_context.fullname, &st) <0)
    {
        goto finish;
    }
    
    if (st.st_size > g_log_size)
    {
        char newfile[PATH_MAX] = {0};
        
        fclose(g_log_context.fd);
        g_log_context.fd = NULL;
        snprintf(newfile, PATH_MAX, "%s.bak", g_log_context.fullname);
        if (rename(g_log_context.fullname, newfile) != 0)
        {
            fprintf(stderr,"rename %s to %s error\n", g_log_context.fullname, newfile);
        }

        if((g_log_context.fd = fopen(g_log_context.fullname, "a+")) == NULL)
        {
            g_log_context.fd = stdout;
        }
    }

finish:
    pthread_mutex_unlock(&g_log_context.log_mutex);
}


void log_print(cwmp_log_level_t level, const char * file, const char * func, int line, const char *fmt, va_list ap)
{
    char            *logtype;
    struct timeb    timebuffer;
    struct tm       *tt;
    char            buf[MAX_LOGBUF_LENGTH+1] = {0};
    size_t          log_len = 0;
    
    if(g_log_context.enable_log == FALSE)
    {
        return;
    }
    
    if (level > g_log_context.level)
    {
        return;
    }

    switch (level)
    {
    case CWMP_LOG_EMERG:
        logtype = "EMER";
        break;

    case CWMP_LOG_ALERT:
        logtype = "ALER";
        break;

    case CWMP_LOG_CRIT:
        logtype = "CRIT";
        break;

    case CWMP_LOG_ERROR:
        logtype = "ERRO";
        break;

    case CWMP_LOG_WARN:
        logtype = "WARN";
        break;

    case CWMP_LOG_NOTICE:
        logtype = "NOTI";
        break;

    case CWMP_LOG_INFO:
        logtype = "INFO";
        break;
        
    case CWMP_LOG_DEBUG:
        logtype = "DBUG";
        break;

    default:
        logtype = "INFO";
        level = CWMP_LOG_INFO;
    }

    struct tm p;
    ftime(&timebuffer);
    tt = localtime_r(&timebuffer.time, &p);
    
    //if(level > CWMP_LOG_INFO)
    if(level > CWMP_LOG_CRIT)
    {
        snprintf(buf, MAX_LOGBUF_LENGTH, "%04d-%02d-%02d %02d:%02d:%02d.%3d [%s] [%s:%s()] [%04d] ",
                                        tt->tm_year+1900,
                                        tt->tm_mon+1,
                                        tt->tm_mday,
                                        tt->tm_hour,
                                        tt->tm_min,
                                        tt->tm_sec,
                                        timebuffer.millitm,
                                        logtype, file, func, line);
    }
    else
    {
        snprintf(buf, MAX_LOGBUF_LENGTH, "%04d-%02d-%02d %02d:%02d:%02d.%3d [%s] ",                                         
                                        tt->tm_year+1900,
                                        tt->tm_mon+1,
                                        tt->tm_mday,
                                        tt->tm_hour,
                                        tt->tm_min,
                                        tt->tm_sec,
                                        timebuffer.millitm,
                                        logtype);        
    }                                         
    log_len = strlen(buf);
    vsnprintf(buf + log_len, MAX_LOGBUF_LENGTH - log_len, fmt, ap);

    log_len = strlen(buf);
    if (buf[log_len - 1] != '\n')
    {
        buf[log_len] = '\n';
        ++log_len;
        buf[log_len] = 0;
    }

    log_write(buf,log_len);
    log_checkfile();
    
    if (g_log_context.enable_syslog)
    {
        if(pthread_mutex_lock(&g_log_context.log_mutex) == 0)
        {
            syslog(LOG_USER|level, buf);
            pthread_mutex_lock(&g_log_context.log_mutex);
        }
    }
}

void log_emerg(const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_EMERG, file, func, line, fmt, ap);
    va_end(ap);
}

void log_alert( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_ALERT, file, func, line, fmt, ap);
    va_end(ap);
}

void log_crit( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_CRIT, file, func, line, fmt, ap);
    va_end(ap);
}

void log_error( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_ERROR, file, func, line, fmt, ap);
    va_end(ap);
}

void log_warn( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_WARN, file, func, line, fmt, ap);
    va_end(ap);
}

void log_notice( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_NOTICE, file, func, line, fmt, ap);
    va_end(ap);
}

void log_info( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_INFO, file, func, line, fmt, ap);
    va_end(ap);
}

void log_debug( const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(CWMP_LOG_DEBUG, file, func, line, fmt, ap);
    va_end(ap);
}

void loging(int type,  const char * file, const char * func, int line, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_print(type, file, func, line, fmt, ap);
    va_end(ap);
}



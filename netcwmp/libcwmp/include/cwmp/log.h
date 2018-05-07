#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <syslog.h>
#include <limits.h>

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#define DEFAULT_LOG_PATHNAME            "/var/log/cwmpd.log"          //默认log路径
#define DEFAULT_LOG_LEVEL           CWMP_LOG_DEBUG    //默认level为DEBUG
#define DEFAULT_LOG_ENABLE          TRUE            //默认log开启
#define DEFAULT_LOG_ENABLE_SYS      FALSE           //默认syslog不开启


typedef enum cwmp_log_level
{
    CWMP_LOG_EMERG = LOG_EMERG,   //The message says the system is unusable. 
    CWMP_LOG_ALERT = LOG_ALERT,   //Action on the message must be taken immediately.     
    CWMP_LOG_CRIT  = LOG_CRIT,    //The message states a critical condition.     
    CWMP_LOG_ERROR = LOG_ERR,     //The message describes an error. 
    CWMP_LOG_WARN  = LOG_WARNING, //The message is a warning. 
    CWMP_LOG_NOTICE= LOG_NOTICE,  //The message describes a normal but important event. 
    CWMP_LOG_INFO  = LOG_INFO,    //The message is purely informational. 
    CWMP_LOG_DEBUG = LOG_DEBUG    //The message is only for debugging purposes. 
}cwmp_log_level_t;


struct cwmp_log_st
{
    FILE                          *fd;	
  	cwmp_log_level_t      level;
    int                              enable_log;
    int                              enable_syslog;
    char                           fullname[PATH_MAX];
    pthread_mutex_t       log_mutex;
};


typedef struct cwmp_log_st cwmp_log_t;


#define MODULE_NAME_LEN     128         //模块名称的最大长度

int init_log(const char *path_name, int enable_log, cwmp_log_level_t level,  int enable_syslog, long size);
 int cwmp_set_log_param(const char *path_name, cwmp_log_level_t level, int enable_log, int enable_syslog, long size);
void log_print(cwmp_log_level_t level, const char * file, const char * func, int line, const char *fmt, va_list ap);

void log_emerg(const char * file, const char * func, int line, const char * fmt, ...);
void log_alert( const char * file, const char * func, int line, const char * fmt, ...);
void log_critical(const char * file, const char * func, int line, const char * fmt, ...);
void log_error( const char * file, const char * func, int line, const char * fmt, ...);
void log_warn(const char * file, const char * func, int line, const char * fmt, ...);
void log_notice(const char * file, const char * func, int line, const char * fmt, ...);
void log_info(const char * file, const char * func, int line, const char * fmt, ...);
void log_debug(const char * file, const char * func, int line, const char * fmt, ...);

#define cwmp_log_debug(x...)          log_debug(__FILE__,__func__, __LINE__, ##x)     
#define cwmp_log_info(x...)              log_info(__FILE__,__func__, __LINE__, ##x)     
#define cwmp_log_error(x...)           log_error(__FILE__,__func__, __LINE__, ##x)     
#define cwmp_log_alert(x...)            log_alert(__FILE__,__func__, __LINE__, ##x)     
#define cwmp_log_critical(x...)         log_critical(__FILE__,__func__, __LINE__, ##x)     
#define FUNCTION_TRACE()         	  cwmp_log_debug("TRACE:\n")

#ifdef __cplusplus
}
#endif

#endif


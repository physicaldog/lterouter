/************************************************************************
 *                                                                      *
 * Netcwmp/Opencwmp Project                                             *
 * A software client for enabling TR-069 in embedded devices (CPE).     *
 *                                                                      *
 * Copyright (C) 2013-2014  netcwmp.netcwmp group                            *
 *                                                                      *
 * This program is free software; you can redistribute it and/or        *
 * modify it under the terms of the GNU General Public License          *
 * as published by the Free Software Foundation; either version 2       *
 * of the License, or (at your option) any later version.               *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU Lesser General Public     *
 * License along with this library; if not, write to the                *
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,          *
 * Boston, MA  02111-1307 USA                                           *
 *                                                                      *
 * Copyright 2013-2014  Mr.x(Mr.x) <netcwmp@gmail.com>          *
 *                                                                      *
 ***********************************************************************/

#include "cwmpd.h"
#include "cwmp/log.h"

#define CWMP_VALUE_UNSET -1



int              cwmp_argc;
char           **cwmp_argv;


static pool_t * cwmp_global_pool;



void cwmp_daemon()
{
    //daemon(0, 1);
}




void cwmp_getopt(int argc, char **argv)
{
    
}


static int cwmp_save_argv( int argc, char *const *argv)
{
    cwmp_argv = (char **) argv;
    cwmp_argc = argc;

    return 0;
}


int cwmp_set_var(cwmp_t * cwmp)
{
    FUNCTION_TRACE();


    cwmp_bzero(cwmp, sizeof(cwmp_t));
    cwmp->new_request = CWMP_TRUE;
    pool_t * pool = pool_create(POOL_DEFAULT_SIZE);
    cwmp->pool = pool;


    cwmp_event_init(cwmp);

    cwmp->queue = queue_create(pool);

    return CWMP_OK;
}




#ifdef USE_CWMP_OPENSSL
void cwmp_init_ssl(cwmp_t * cwmp)
{
    char * cafile = cwmp_conf_pool_get(cwmp_global_pool, "cwmp:ca_file");
    char * capasswd = cwmp_conf_pool_get(cwmp_global_pool, "cwmp:ca_password");   
    cwmp->ssl_ctx = openssl_initialize_ctx(cafile, capasswd);
}
#endif

//初始化日志，从配置文件中取得日志的配置
int cwmp_init_log(cwmp_t * cwmp)
{
    int log_enable;
    int syslog_enable;
    int log_size;
    int level;    
    char *path_name;
    pool_t *pool = cwmp->pool;

    log_enable = cwmp_conf_get_int("log:enable");
    level = cwmp_conf_get_int("log:level");
    log_size = cwmp_conf_get_int("log:size");
    syslog_enable = cwmp_conf_get_int("log:syslog");

    path_name = cwmp_conf_pool_get(pool, "log:path"); 
    
    printf("path_name=%s,  level=%d, log_enable=%d, syslog=%d, log_size=%d\n", 
            path_name,  log_enable, level, syslog_enable,log_size);
    init_log(path_name, log_enable, level, syslog_enable, log_size);    
    return TRUE;
}


int main(int argc, char **argv)
{
    cwmp_pid_t pid;
    cwmp_t * cwmp;
    
    int cwmp_enable = 0;

#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    signal(SIGPIPE, SIG_IGN);

    pid = fork();

    if(pid == 0)
    {
    // cwmp_log_init("/var/log/cwmpd.log", CWMP_LOG_INFO);
    // cwmp_log_init(NULL, CWMP_LOG_INFO);
    cwmp_global_pool = pool_create(POOL_DEFAULT_SIZE);
    cwmp = pool_palloc(cwmp_global_pool, sizeof(cwmp_t));

    cwmp_conf_open("/etc/cwmp.conf");
    
    cwmp_enable=cwmp_conf_get_int("cwmp:enable");
    if(!cwmp_enable)
    {
        exit(-1);    
    }

    cwmp_getopt(argc, argv);
    //cwmp_init_db();    

    cwmp_set_var(cwmp);
    cwmp_daemon();
    
    //初始化日志，从配置文件中取得日志的配置
    cwmp_init_log(cwmp);   

    // 初始化cwmp配置
    cwmp_conf_init(cwmp);

#ifdef USE_CWMP_OPENSSL
    cwmp_init_ssl(cwmp);
#endif

    cwmp_model_load(cwmp, "/etc/device.xml");
    cwmp_process_start_master(cwmp);

    return 0;
     }

    sleep(30);
    return 0;     
}




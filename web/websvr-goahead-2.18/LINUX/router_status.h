/*************************************************************************
	> File Name: router_status.h
	> Author: sunxueqian
	> Mail: physicalmonkey@outlook.com
 ************************************************************************/
#ifndef _ROUTER_STATUS_H_
#define _ROUTER_STATUS_H_

#define SerPort "ttyem300"

/* the forms procedure routine */
//void deviceInfo(webs_t wp, char_t *path, char_t *query);
//void CPUUsageRate(webs_t wp, char_t *path, char_t *query);

int getConfig(char *Config, char *buff);
void apn_query(webs_t wp, char_t *path, char_t *query);
void lanip_query(webs_t wp, char_t *path, char_t *query);

#endif

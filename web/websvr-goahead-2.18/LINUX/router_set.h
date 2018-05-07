/*************************************************************************
	> File Name: router_set.h
	> Author: sunxueqian
	> Mail: physicalmonkey@outlook.com
 ************************************************************************/
#ifndef _ROUTER_SET_H_
#define _ROUTER_SET_H_
#define SerPort "ttyem300"


/* the forms procedure routine */

void apn_set(webs_t wp, char_t *path, char_t *query);
void lanip_set(webs_t wp, char_t *path, char_t *query);

void sysReset(webs_t wp, char_t *path, char_t *query);
void moduleReset(webs_t wp, char_t *path, char_t *query);

#endif

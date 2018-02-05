/*************************************************************************
	> File Name: a6g2cimpl.h
	> Author: huaqiyan
	> Mail: huaqiyan@zlg.cn
	> Created Time: Tue 01 Jul 2014 08:42:21 AM CST
 ************************************************************************/
#ifndef _A6G2C_IMPL_H_
#define _A6G2C_IMPL_H_

#define MAX_LEN_HTML_LINE 4096

/* the forms procedure routine */
void on_gp34_set(webs_t wp, char_t *path, char_t *query);

void on_led_set(webs_t wp, char_t *path, char_t *query);
void on_buzzer_set(webs_t wp, char_t *path, char_t *query);

void on_bkl_set(webs_t wp, char_t *path, char_t *query);

void on_response(webs_t wp, char *webroot);

#endif

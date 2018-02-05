/*************************************************************************
	> File Name: a6g2cimpl.c
	> Author: huaqiyan
	> Mail: huaqiyan@zlg.cn
	> Created Time: Tue 01 Jul 2014 08:42:21 AM CST
 ************************************************************************/
/*
 * 	Vendor program for the GoAhead WebServer. This is a demonstration
 *	program to control the device with web server.
 */

#include	"../uemf.h"
#include	"../wsIntrn.h"
#include	<signal.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>

#ifdef WEBS_SSL_SUPPORT
#include	"../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include	"../um.h"
void	formDefineUserMgmt(void);
#endif

#include "a6g2cimpl.h"

/*
 * src - the stream line will be processed
 * sub - substring name location in src
 * devfile - device file against the input element
 * name - the input name in the html stream line
 */
static char get_gpio_dir(char* src, char* sub, char* devfile, char* name, char enabled)
{
	char tmp[256], *p, cv[10] = {'\0','\0','\0','\0'};
	FILE* fd;
	char sb = 0;
	
	// replace this line with the result
	fd = fopen(devfile,"r");
	if(fd)
	{
		// read the value
		fgets(cv, 10, fd);
		fclose(fd);
	}

	if(strstr(cv,"out") != NULL)
	{
		p = sub + strlen(name);
		sprintf(tmp,"%s", p);
		*p='\0';
		if(!enabled)
			sprintf(p,"%s%s"," checked disabled", tmp);
		else
			sprintf(p,"%s%s"," checked", tmp);

		sb = 1;
	}
	else
	{
		p = sub + strlen(name);
		sprintf(tmp,"%s", p);
		*p='\0';
		if(!enabled)
			sprintf(p,"%s%s"," disabled", tmp);
		else
			sprintf(p,"%s", tmp);

		sb = 0;
	}
	//printf("%s: %s, sb=%d\n", devfile, cv, sb);
	return sb;
}

/*
 * src      - the stream line will be processed
 * sub      - substring name location in src
 * devfile  - device file against the input element
 * name     - the input name in the html stream line
 * disabled - disabled or enabled the input control
 *
 * return the input current state: non zero is on / zero is off
 *
 */
static char get_gpio_val(char* src, char* sub, char* devfile, char* name, char enabled)
{
	char tmp[256], *p, cv[10] = {'\0','\0','\0','\0'};
	FILE* fd;
	char sb = 0;
	
	// replace this line with the result
	fd = fopen(devfile,"r");
	if(fd)
	{
		// read the value
		fgets(cv, 10, fd);
		fclose(fd);
	}

	if(strstr(cv,"1") != NULL)
	{
		p = sub + strlen(name);
		sprintf(tmp,"%s", p);
		*p='\0';
		if(!enabled)
			sprintf(p,"%s%s"," checked disabled", tmp);
		else
			sprintf(p,"%s%s"," checked", tmp);

		sb = 1;
	}
	else
	{
		p = sub + strlen(name);
		sprintf(tmp,"%s", p);
		*p='\0';
		if(!enabled)
			sprintf(p,"%s%s"," disabled", tmp);
		else
			sprintf(p,"%s", tmp);

		sb = 0;
	}
	//printf("%s: %s, sb=%d\n", devfile, cv, sb);
	return sb;
}

static char* get_bkl_val(char* src, char* sub, char* devfile, char* name)
{
	char tmp[256], *p;
	char cv[10] = {'8','0','\0','\0'};
	FILE* fd;
	
	// replace this line with the result
	fd = fopen(devfile,"r");
	if(fd)
	{
		// read the value
		fgets(&cv[0], 10, fd);
		fclose(fd);
	}

	p = sub + strlen(name);
	// <input .... value="xx" .../>
	p = strstr(src, "value=");
	p += strlen("value=");
	*p++='\0';

	// find the next space following 'value="xx"'
	p = strchr(p,' ');

	// insert the result
	sprintf(tmp,"\"%s\"%s", cv, p);

	strcat(src, tmp);
	return src;
}

void on_response(webs_t wp, char *webroot)
{
	char *buf, tmp[256];

	FILE *fin;
	char *s1, *s2, sb;

	a_assert(websValid(wp));

	buf = malloc(MAX_LEN_HTML_LINE);
	if(!buf)
		return;

	memset(buf, 0, MAX_LEN_HTML_LINE);

	// response, send the result html stream to the browser
	sprintf(tmp,"%s/%s", webroot, "resphtml.tm");
	fin = fopen(tmp,"r");
	if(fin)
	{
		websWrite(wp, T("HTTP/1.0 200 OK\n"));

		/*
		 *	By license terms the following line of code must not be modified
		 */
		websWrite(wp, T("Server: %s\r\n"), WEBS_NAME);

		websWrite(wp, T("Pragma: no-cache\n"));
		websWrite(wp, T("Cache-control: no-cache\n"));
		websWrite(wp, T("Content-Type: text/html\n"));
		websWrite(wp, T("\n"));

		s1 = buf;
		while(!feof(fin))
		{
			if(fgets(s1, MAX_LEN_HTML_LINE, fin))
			{
				// get the input elements value from device

				// gpio-p3.4 direction
				if((s2 = strstr(s1,"name=\"dir34\"")) != NULL)
				{
					// replace this line with the result
					sb = get_gpio_dir(s1, s2, "/sys/class/gpio/gpio68/direction", "name=\"dir34\"", 1);
					websWrite(wp, T("%s\n"), s1);

				}
				// gpio-p3.4 value
				else if((s2 = strstr(s1,"name=\"val34\"")) != NULL)
				{
					// replace this line with the result
					get_gpio_val(s1, s2, "/sys/class/gpio/gpio68/value", "name=\"val34\"", sb);
					websWrite(wp, T("%s\n"), s1);
				}

				// led value
				else if((s2 = strstr(s1,"name=\"val_led\"")) != NULL)
				{
					// replace this line with the result
					get_gpio_val(s1, s2, "/sys/class/leds/led-err/brightness", "name=\"val_led\"", 1);
					websWrite(wp, T("%s\n"), s1);
				}
				// buzzer value
				else if((s2 = strstr(s1,"name=\"val_buz\"")) != NULL)
				{
					// replace this line with the result
					get_gpio_val(s1, s2, "/sys/class/leds/beeper-pwm/brightness", "name=\"val_buz\"", 1);
					websWrite(wp, T("%s\n"), s1);
				}
				// backlight value
				else if((s2 = strstr(s1,"name=\"bkl_val\"")) != NULL)
				{
					// replace this line with the result
					s1 = get_bkl_val(s1, s2, "/sys/class/backlight/backlight/brightness", "name=\"bkl_val\"");
					websWrite(wp, T("%s\n"), s1);
				}
				else
				{
					websWrite(wp, T("%s\n"), s1);
				}
			}
		}

		free(buf);
		fclose(fin);

	} //! if(fin)
}


//
// goahead form action procedure routines
//
void on_gp34_set(webs_t wp, char_t *path, char_t *query)
{
	char *io_dir, *io_val;

	a_assert(websValid(wp));

	if(query)
	{	
		io_dir = gstrstr(query,"dir34=on");
		io_val = gstrstr(query,"val34=on");

		if(io_dir)
		{
			// direction is set to "out"
			system("echo out > /sys/class/gpio/gpio68/direction");
			if(io_val)
				system("echo 1 > /sys/class/gpio/gpio68/value");
			else
				system("echo 0 > /sys/class/gpio/gpio68/value");
		}
		else
		{
			// direction is set to "in"
			system("echo in > /sys/class/gpio/gpio68/direction");
		}
	}

	// response, send the result html stream to the browser
	on_response(wp, "/home/webroot");
	websDone(wp, 200);
	return;
}


void on_led_set(webs_t wp, char_t *path, char_t *query)
{
	// get the input value in query stream
	char *io_val;

	a_assert(websValid(wp));

	if(query)
	{	
		io_val = gstrstr(query,"val_led=on");

		if(io_val)
			system("echo 1 > /sys/class/leds/led-err/brightness");
		else
			system("echo 0 > /sys/class/leds/led-err/brightness");
	}

	// response, send the result html stream to the browser
	on_response(wp, "/home/webroot");
	websDone(wp, 200);
	return;
}

void on_buzzer_set(webs_t wp, char_t *path, char_t *query)
{
	// get the input value in query stream
	char *io_val;

	a_assert(websValid(wp));

	if(query)
	{	
		io_val = gstrstr(query,"val_buz=on");

		if(io_val)
			system("echo 1 > /sys/class/leds/beeper-pwm/brightness");
		else
			system("echo 0 > /sys/class/leds/beeper-pwm/brightness");
	}

	// response, send the result html stream to the browser
	on_response(wp, "/home/webroot");
	websDone(wp, 200);
	return;
}

void on_bkl_set(webs_t wp, char_t *path, char_t *query)
{
	// get the input value in query stream
	char *val;
	char tmp[256];

	a_assert(websValid(wp));

	if(query)
	{	
		val = websGetVar(wp, T("bkl_val"), T("80"));
		sprintf(tmp,"echo %s > /sys/class/backlight/backlight/brightness", val);
        system(tmp);
		
		// response, send the result html stream to the browser
		on_response(wp, "/home/webroot");
		websDone(wp, 200);
	}
	return;
}

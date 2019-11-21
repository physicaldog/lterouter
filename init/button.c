#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <sys/reboot.h>
#include "common.h"

#define GPIOKEY_EVENT_PATH	"/dev/input/event1"
long down_time = 0;
long up_time = 0;

void handle_key(void)
{
	int ret = 0;
	system("/usr/local/key-handle.sh");
	ret = up_time - down_time;
	if(ret > 5) {
		log_msg("\nsystem reset ret = %d\n",ret);
		system("cd /opt/upgrade;tar -xf config.tar -C /opt/");
		sync();
		reboot(RB_AUTOBOOT);
	}
	else {
		log_msg("\nsystem reboot ret = %d\n",ret);
		sync();
		reboot(RB_AUTOBOOT);
	}
}

int main(void)
{
	int gpiokey_fd;
	int event_len;
	struct input_event event;

	gpiokey_fd = open(GPIOKEY_EVENT_PATH, O_RDONLY);
	if(gpiokey_fd < 0) {
		log_msg("open gpiokey_event error\n");
		return -1;
	}

	event_len = sizeof(struct input_event);
	for(;;) {
		if(read(gpiokey_fd, &event, event_len) == event_len) {
			if(event.type == EV_KEY) {
				if(event.code == KEY_CONFIG) {
					if(event.value == 1) { // key press
						down_time = event.time.tv_sec;
						log_msg("key press,%ld\n",down_time);
						
					}else if(event.value == 0) { // key press
						up_time = event.time.tv_sec;
						log_msg("key up,%ld\n",up_time);
						handle_key();
					}
				}
			}
		}
	}

	return 0;
}

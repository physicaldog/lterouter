#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

int fd;
int ecm_done;
int net_sta;
int ndis_sta;


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <linux/rtc.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    
    struct  timeval    tv;
    struct  timezone   tz;
    int ch;
	int rc, fd;
	time_t secs = 0;
	struct tm rtc_tm;;
    int opt = 0;
    while((ch = getopt(argc,argv,"rd")) != -1) {
        switch(ch)
        {
            case 'r':
                opt = 1;
                break;
            case 'd':
                opt = 2;
                break;
            default:
                break;
        }
    }
    gettimeofday(&tv,&tz);
    if (opt == 2)
    {
        printf("tv_sec:%ld\n",tv.tv_sec);
        printf("tv_usec:%ld\n",tv.tv_usec);
        printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
        printf("tz_dsttime:%d\n",tz.tz_dsttime);
    }

    char src[64]={0}, dest[64]={0};
    uint64_t millis = (tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000);
    sprintf(src, "%lld", millis);
    strcat(dest, "echo ");
    strcat(dest, src);
    strcat(dest, "> /tmp/system_msec");
    system(dest);
    
    if (opt == 2)
    {
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME, &tp);
        printf("clock tv_sec %02ld \n", tp.tv_sec);
        printf("clock tv_nsec %09lu \n", tp.tv_nsec);
    }
   
	return -1;
}

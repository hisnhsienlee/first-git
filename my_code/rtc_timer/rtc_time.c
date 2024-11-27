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
#define SEC_TO_MSEC(s)	((s) * 1000ULL)
int main(int argc, char *argv[])
{
    int ch;
	int rc, fd;
	time_t secs = 0;
	struct tm rtc_tm;;
    int opt = 0;
    while((ch = getopt(argc,argv,"rs")) != -1) {
        switch(ch)
        {
            case 'r':
                opt = 1;
                break;
            case 's':
                opt = 2;
                break;
            default:
                break;
        }
    }
    fd = open("/dev/rtc0", O_RDONLY);
	if(fd < 0) {
		printf("rtc_time: Unable to open RTC device errno: %d\n", errno);
		return -1;
	}

	rc = ioctl(fd,RTC_RD_TIME,&rtc_tm);
	if(rc < 0) {
		printf("rtc_time: Unable to read from RTC device\n");
		goto fail_rtc;
	} 

    if (opt == 1)
    {
        printf("%d/%d/%d %d:%d:%d\n", rtc_tm.tm_mon+1, rtc_tm.tm_mday, rtc_tm.tm_year+1900, rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
    } else if (opt == 2) {
        /* Convert the time to UTC and then to milliseconds and store it */
        secs = mktime(&rtc_tm);
        secs += rtc_tm.tm_gmtoff;
        //printf("rtc_time: secs = %ld\r\n", secs);
        int64_t msecs =0;
        msecs = SEC_TO_MSEC(secs);
        printf("rtc_time: mseconds = %lld\n", msecs);
    }
    close(fd);
    return 0;
fail_rtc:
	close(fd);
	return -1;
}

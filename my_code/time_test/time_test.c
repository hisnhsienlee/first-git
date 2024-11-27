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

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

//#define LOOP_TIMES   10000000
#define LOOP_TIMES   5000000
#define GTOD_FLAG    ~0

const char* timetype_str[7] = {
"CLOCK_REALTIME",
"CLOCK_MONOTONIC",
"CLOCK_THREAD_CPUTIME_ID",
"CLOCK_PROCESS_CPUTIME_ID",
"CLOCK_MONOTONIC_RAW",
"CLOCK_REALTIME_COARSE",
"CLOCK_MONOTONIC_COARSE"     
};

// flag GTOD_FLAG getimeofday
static inline void do_gettime(unsigned int flag, int loop_times)
{
    sleep(60*10);
    /*int i = 0;
    struct timeval tv;
    struct timespec time1 = {0, 0};
    if (flag == GTOD_FLAG) {
        for (i = 0; i < loop_times; i++) {
            gettimeofday (&tv, NULL);
        }

    } else {
        for (i = 0; i < loop_times; i++) {
            clock_gettime(flag, &time1);
        }
    }*/
}

// flag ~0 gettimeofday,  flag 0 - 3, clock_gettime
int gettime_test(unsigned int flag, int loop_times)
{
    int i = 0;
    long s_intval;
    long us_intval;
    long interval = 0;
    struct timeval begin;
    struct timeval end;

    gettimeofday (&begin, NULL);
    do_gettime(flag, loop_times);
    gettimeofday (&end, NULL);

    s_intval = end.tv_sec - begin.tv_sec;
    us_intval = end.tv_usec - begin.tv_usec;

    interval = s_intval * 1000 + us_intval/1000;
    printf("%d times cost %ldms\n", loop_times, interval);

    return 0;
}
void gettimeofday_test(int loop_times)
{
    printf("gettimeofday\n");
    gettime_test(GTOD_FLAG, loop_times);
    printf("\n");
}

void clock_gettime_test(int loop_times, unsigned int flag)
{
    printf("clock_gettime type: %s\n", timetype_str[flag]);
    gettime_test(flag, loop_times);
    printf("\n");
}

int main()
{
    int flag = 0;
    gettimeofday_test(LOOP_TIMES);
    for (flag = 0; flag < 7; flag++)
        clock_gettime_test(LOOP_TIMES, flag); 

    return 0;
}

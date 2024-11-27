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

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;
time_t secs = 0;
int debug_opt = 0;
typedef enum time_bases
{
	ATS_RTC = 0, /**< Real time clock timebase.*/
	ATS_TOD,     /**< Proxy base for number of bases.*/
	ATS_USER,    /**< User timebase. */
	ATS_SECURE,  /**< Secure timebase. */
	ATS_DRM,     /**< Digital rights management timebase. */
	ATS_USER_UTC,   /**< Universal Time Coordinated user
			  timebase. */
	ATS_USER_TZ_DL, /**< Global time zone user timebase. */
	ATS_GPS,    /**< Base for GPS time. \n
		      @note1hang When
		      ATS_GSTK is modified,
		      changes are also
		      reflected
		      on
		      ATS_TOD.
		      */
	ATS_1X,     /**< Base for 1X time. \n
		      @note1hang When
		      ATS_1X is modified,
		      changes are also
		      reflected
		      on
		      ATS_TOD.
		      */
	ATS_HDR,    /**< Base for HDR time. \n
		      @note1hang When
		      ATS_HDR is
		      modified, changes
		      are also
		      reflected
		      on
		      ATS_TOD.
		      */
	ATS_WCDMA,  /**< Base for WCDMA time. \n
		      @note1hang When
		      ATS_WCDMA is
		      modified,
		      changes are
		      also
		      reflected
		      on
		      ATS_TOD.
		      */
	ATS_MFLO,   /**< Base for MediaFLO time. \n
		      @note1hang
		      When ATS_MFLO
		      is modified,
		      changes are
		      also
		      reflected
		      on
		      ATS_TOD.
		      */
	ATS_TOD_GEN,   /**< Base for APPS time. \n
		      */
	ATS_INVALID = 0x10000000
} time_bases_type;

/*
 * Time unit type.
 */
typedef enum time_unit
{
	TIME_STAMP,      /**< Time is in timestamp format. */
	TIME_MSEC,       /**< Time is in millisecond format. */
	TIME_SECS,       /**< Time is in second format. */
	TIME_JULIAN,     /**< Time is in Julian calendar format. */
	TIME_20MS_FRAME, /**< Time is in 20-millisecond frames
			   format. */
	TIME_NONE        /**< Time format is undefined. */
} time_unit_type ;


typedef enum time_genoff_opr
{
	T_SET, /**< Genoff_Opr sets the time. */
	T_GET, /**< Genoff_Opr gets the time. */
	T_IS_SET, /**< Genoff_Opr checks offset set. */
	T_DISABLE, /**< Disable Logging. */
	T_ENABLE, /**< Enable Logging. */
    T_WNC_GET,
	T_MAX  /**< Placeholder for maximum enumerator value. */
} time_genoff_opr_type;

typedef struct ind_offset {
	int offset;             /* Offset number */
	int ind_received;       /* Flag for indication status */
	pthread_mutex_t lock;   /* Lock to protect structure */
} cb_indication_t;

#define OFFSET_LOCATION "/data/time"
#define MAX_CB_FUNC              10
#define TIME_GENOFF_REMOTE_UPDATE_MS  10
#define TIME_GENOFF_UPDATE_THRESHOLD_MS 10
#define GENOFF_SOCKET_NAME	"#time_genoff"
#define GENOFF_MAX_CONCURRENT_CONN	4

#define SEC_TO_MSEC(s)	((s) * 1000ULL)
#define MSEC_TO_SEC(s)	((s) / 1000ULL)
#define USEC_TO_MSEC(s)	((s) / 1000ULL)
#define NSEC_TO_MSEC(s)	((s) / 1000000ULL)
#define	MODEM_EPOCH_DIFFERENCE	 315964800
#define ATS_MAX		ATS_TOD_GEN + 1
#define FILE_NAME_MAX	100
#define strlcpy		g_strlcpy

/* Dividend of factor to multiply for converting sclk to cx32 */
#define TIME_CONV_SCLK_CX32_DIVIDEND 256
/* Divisor of factor to multiply for converting sclk to cx32 */
#define TIME_CONV_SCLK_CX32_DIVISOR 125
/* Conversion from cx32 to 1.25ms */
#define TIME_CONV_CX32_PER_1p25MS 0xC000

typedef enum time_persistant_operation {
	TIME_READ_MEMORY,
	TIME_WRITE_MEMORY
}time_persistant_opr_type;

/* Time Generic Offset 'type 1' callback function */
typedef int (*time_genoff_t1_cb_type)(void);

typedef struct {
	/* Mark as TRUE if persistent storage is needed */
	uint8_t initialized;
	/* file name for efs item file */
	char f_name[100];
	/* Threshold in ms for writing back in efs item file */
	int64_t threshold;
} time_genoff_per_storage_type;

/* Time Generic Offset pointer type */
typedef struct time_genoff_struct *time_genoff_ptr;

typedef struct time_genoff_struct{
	/* Generic Offset, always stored in ms */
	int64_t generic_offset;
	/* Flag to indicate if time-of-day has ever been set before */
	uint8_t initialized;
	/* Time base Type */
	time_bases_type bases_type;
	/* Initialization Sequence */
	time_genoff_t1_cb_type init_func;
	/* Mark if subsys based on another subsys */
	uint8_t reqd_base_genoff;
	/* Base Subsys */
	time_bases_type subsys_base;
	/* Specification for persistent storage */
	time_genoff_per_storage_type    per_storage_spec;
} time_genoff_struct_type;

struct send_recv_struct {
	uint32_t base;
	uint32_t unit;
	uint32_t operation;
	uint64_t value;
	int result;
};

typedef struct time_genoff_info {
	time_bases_type base;		/* Genoff in consideration */
	void *ts_val;			/* Time to be set/get */
	time_unit_type unit;		/* Time unit */
	time_genoff_opr_type operation; /* Time operation to be done */
}time_genoff_info_type;

typedef struct{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}_time;

void time_timestamp_to_time(time_t timestamp)
{
    _time time;
    struct tm *t;
    
    t = localtime(&timestamp);
    
    time.year = t->tm_year+1900;
    time.month = t->tm_mon+1;
    time.day = t->tm_mday;
    time.hour = t->tm_hour;
    time.minute = t->tm_min;
    time.second = t->tm_sec;
    printf("Lib:%s:  %d-%d-%d %d:%d:%d\n", __func__, time.year, time.month, time.day, time.hour, time.minute, time.second);
}

int time_genoff_operation(time_genoff_info_type *pargs)
{
	int sock_id, length;
	struct sockaddr_un time_socket;
	struct send_recv_struct to_send;
	struct tm *time_julian, intermediate_result;

    if (debug_opt) {
        printf("Lib:%s: pargs->base = %d\n", __func__, pargs->base);
        printf("Lib:%s: pargs->ts_val = %llu\n", __func__, *(uint64_t *)(pargs->ts_val));
        printf("Lib:%s: pargs->operation = %d\n", __func__,pargs->operation);
    }

	/* Validity check */
	if ( (pargs->base >= ATS_MAX) ||
		!(pargs->unit > TIME_STAMP && pargs->unit < TIME_20MS_FRAME) ||
		(pargs->operation >= T_MAX) ||
		(pargs->ts_val == NULL)) {
        printf("Lib:%s: Invalid input arguments\n", __func__);
		return -EINVAL;
	}

	sock_id = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock_id < 0) {
		printf("Lib:%s: Unable to create socket\n", __func__);
		return -EINVAL;
	}
	time_socket.sun_family = AF_UNIX;
	strncpy(time_socket.sun_path, GENOFF_SOCKET_NAME, UNIX_PATH_MAX);
	/* Create abstract domain socket */
	time_socket.sun_path[0] = 0;
	length = strlen(GENOFF_SOCKET_NAME) + sizeof(time_socket.sun_family);
	if (connect(sock_id, (struct sockaddr *)&time_socket, length) == -1) {
		printf("Lib:%s: Connection failed !!\n", __func__);
		goto error_close_socket;
	}

	/*
	 * Format to send : base, unit, operation, value (for set)
	 * Time sent/received in msecs. Later converted to required format.
	 */
	if (pargs->operation == T_GET) {
		to_send.base = pargs->base;
		to_send.unit = TIME_MSEC;
		to_send.operation = T_GET;
		to_send.value = 0;
		to_send.result = -1;

		if (send(sock_id, &to_send, sizeof(to_send), 0) < 0) {
			printf("Lib:%s: Send to server failed !!\n", __func__);
			goto error_close_socket;
		} else {
            if (debug_opt) {
                printf("Lib:%s: Send to server  passed!!\n", __func__);
            }
			if (recv(sock_id, (void *)&to_send, sizeof(to_send), 0) < 0) {
				printf("Lib:%s: Unable to recv data\n",__func__);
				goto error_close_socket;
			}
            if (debug_opt) {
                printf("Lib:Receive Passed == base = %d, unit = %d,"
                    "operation = %d, value = %llu, result = %d\n", to_send.base,
                    to_send.unit, to_send.operation, to_send.value, to_send.result);
            }
			if (to_send.result != 0) {
				printf("Lib: Time Get for Base = %d failed\n",
								to_send.base);
				goto error_close_socket;
			}
            char src[64]={0}, dest[64]={0};
            sprintf(src, "%lld", to_send.value);
            strcat(dest, "echo ");
            strcat(dest, src);
            strcat(dest, "> /tmp/ats_msec");
            system(dest);
            secs = MSEC_TO_SEC(to_send.value);
            time_timestamp_to_time(secs);
		}
		close(sock_id);

		switch(pargs->unit) {
		case TIME_MSEC:
			*(uint64_t *)pargs->ts_val = to_send.value;
			break;
		case TIME_SECS:
			/* convert the time to seconds */
			to_send.value = MSEC_TO_SEC(to_send.value);
			*(uint64_t *)pargs->ts_val = to_send.value;
			break;
		case TIME_JULIAN:
			/* convert the time to julian format */
			to_send.value = MSEC_TO_SEC(to_send.value);
			time_julian = gmtime_r((time_t *)&to_send.value,
					&intermediate_result);
			if (!time_julian) {
				printf("Lib:%s: Invalid time %llu",
						__func__, to_send.value);
				goto error_close_socket;
			}

			memcpy(pargs->ts_val, time_julian, sizeof(struct tm));
			break;
		default:
		        printf("Lib:%s:Invalid time unit %d", __func__, pargs->unit);
			goto error_close_socket;
		}
	} else if (pargs->operation == T_SET) {
		switch(pargs->unit) {
		case TIME_MSEC:
			to_send.value = *(uint64_t *)pargs->ts_val;
			break;
		case TIME_SECS:
			/* Convert the time to msecs */
			to_send.value = *(uint64_t *)pargs->ts_val;
			to_send.value = SEC_TO_MSEC(to_send.value);
			break;
		case TIME_JULIAN:
			/* Convert the time to UTC and then msecs */
			to_send.value= mktime((struct tm *)pargs->ts_val);
			to_send.value = SEC_TO_MSEC(to_send.value);
		        break;
		default:
		        printf("Lib:%s:Invalid time unit %d", __func__, pargs->unit);
			goto error_close_socket;
		}
		to_send.base = pargs->base;
		to_send.unit = TIME_MSEC;
		to_send.operation = T_SET;
		to_send.result = -1;

		if (send(sock_id, &to_send, sizeof(to_send), 0) < 0) {
			printf("Lib:%s: Send to server failed !!\n", __func__);
			goto error_close_socket;
		} else {
            if (debug_opt) {
                printf("Lib:%s: Send to server  passed!!\n", __func__);
            }
			if (recv(sock_id, (void *)&to_send, sizeof(to_send), 0) < 0) {
				printf("Lib:%s: Unable to recv data\n",__func__);
				goto error_close_socket;
			}
            if (debug_opt) {
                printf("Receive Passed == base = %d, unit = %d, operation = %d, result = %d\n",
                to_send.base, to_send.unit, to_send.operation, to_send.result);
            }
			if (to_send.result != 0) {
				printf("Lib: Time Set for Base = %d failed\n",
								to_send.base);
				goto error_close_socket;
			}
		}
		close(sock_id);
    } else if (pargs->operation == T_WNC_GET) {
		to_send.base = pargs->base;
		to_send.unit = TIME_MSEC;
		to_send.operation = T_WNC_GET;
		to_send.value = 0;
		to_send.result = -1;

		if (send(sock_id, &to_send, sizeof(to_send), 0) < 0) {
			printf("Lib:%s: Send to server failed !!\n", __func__);
			goto error_close_socket;
		} else {
            if (debug_opt) {
                printf("Lib:%s: Send to server  passed!!\n", __func__);
            }
			if (recv(sock_id, (void *)&to_send, sizeof(to_send), 0) < 0) {
				printf("Lib:%s: Unable to recv data\n",__func__);
				goto error_close_socket;
			}
            if (debug_opt) {
                printf("Lib:Receive Passed == base = %d, unit = %d,"
                    "operation = %d, value = %llu, result = %d\n", to_send.base,
                    to_send.unit, to_send.operation, to_send.value, to_send.result);
            }
			if (to_send.result != 0) {
				printf("Lib: Time Get for Base = %d failed\n",
								to_send.base);
				goto error_close_socket;
			}
            char src[64]={0}, dest[64]={0};
            sprintf(src, "%lld", to_send.value);
            strcat(dest, "echo ");
            strcat(dest, src);
            strcat(dest, "> /tmp/ats_msec");
            system(dest);
            secs = MSEC_TO_SEC(to_send.value);
            time_timestamp_to_time(secs);
		}
		close(sock_id);

		switch(pargs->unit) {
		case TIME_MSEC:
			*(uint64_t *)pargs->ts_val = to_send.value;
			break;
		case TIME_SECS:
			/* convert the time to seconds */
			to_send.value = MSEC_TO_SEC(to_send.value);
			*(uint64_t *)pargs->ts_val = to_send.value;
			break;
		case TIME_JULIAN:
			/* convert the time to julian format */
			to_send.value = MSEC_TO_SEC(to_send.value);
			time_julian = gmtime_r((time_t *)&to_send.value,
					&intermediate_result);
			if (!time_julian) {
				printf("Lib:%s: Invalid time %llu",
						__func__, to_send.value);
				goto error_close_socket;
			}

			memcpy(pargs->ts_val, time_julian, sizeof(struct tm));
			break;
		default:
		        printf("Lib:%s:Invalid time unit %d", __func__, pargs->unit);
			goto error_close_socket;
		}
	} else {
		printf("Lib:%s: Invalid operation specified\n", __func__);
		goto error_close_socket;
	}

	return 0;

error_close_socket:
	close(sock_id);
	return -EINVAL;
}

void print_info()
{
    printf("-d :debug mode.\n");
    printf("-s <timestamp> :set time.\n");
    printf("-g :get ats + rtc time.\n");
    printf("-m :get modem time .\n");
}

uint64_t string_to_uint64(const char *buff) {
  const char *s = buff;
  // consume leading whitespace
  while (isspace((unsigned char ) *s)) {
    s++;
  }
  int sign = *s;
  if (sign == '-' || sign == '+') {
    s++;
  }
  // Now code knows it the text is "negative"

  // rest of OP's code needs a some work
  char *end;
  errno = 0;
  unsigned long long sl = strtoull(s, &end, 10);

  if (end == s) {
    fprintf(stderr, "%s: not a decimal number\n", buff);
  } else if ('\0' != *end) {
    fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
    // } else if ((sl < 0 || ULONG_MAX == sl) && ERANGE == errno) {
  } else if (sign == '-') {
    fprintf(stderr, "%s negative\n", buff);
    sl = 0;
    errno = ERANGE;
  } else if (ERANGE == errno) {
    fprintf(stderr, "%s out of range of type uint64_t\n", buff);

    // Only needed on rare machines
#if ULLONG_MAX > UINT64_MAX
    else if (sl > UINT64_MAX) {
      fprintf(stderr, "%s out of range of type uint64_t\n", buff);
      sl = UINT64_MAX;
      errno = ERANGE;
    }
#endif

  }
  return (uint64_t) sl;
}

int main(int argc, char *argv[])
{
    int ch;
    int opt, err = 0;
    debug_opt = 0;
    time_genoff_info_type temp_genoff_args={0};
    uint64_t ts_val_secs;
    while((ch = getopt(argc,argv,"mdgs:")) != -1) {
        switch(ch)
        {
            case 'd':
                debug_opt = 1;
                break;
            case 'g':
                //printf("option : g\n");
                opt = 1;
                break;
            case 's':
                ts_val_secs = string_to_uint64(optarg);
                //printf("option : s offset : %s offect_uint64 : %llu\n", optarg, ts_val_secs);
                opt = 2;
                break;
            case 'm':
                opt = 3;
                break;
            default:
                print_info();
                break;
        }
    }
    if ( opt == 1 ) {
        temp_genoff_args.base       = ATS_TOD;
        temp_genoff_args.unit       = TIME_MSEC;
        temp_genoff_args.operation  = T_GET;
        temp_genoff_args.ts_val     = &ts_val_secs;
        time_genoff_operation(&temp_genoff_args);
    } else if ( opt == 2 ) {
        temp_genoff_args.base       = ATS_USER;
        temp_genoff_args.unit       = TIME_MSEC;
        temp_genoff_args.operation  = T_SET;
        temp_genoff_args.ts_val     = &ts_val_secs;
        time_genoff_operation(&temp_genoff_args);
    } else if ( opt == 3 ) {
        temp_genoff_args.base       = ATS_TOD;
        temp_genoff_args.unit       = TIME_MSEC;
        temp_genoff_args.operation  = T_WNC_GET;
        temp_genoff_args.ts_val     = &ts_val_secs;
        time_genoff_operation(&temp_genoff_args);
    }
    return 0;
}


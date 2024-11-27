#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MAP_SIZE 0x80000
#define base 0x780820

/*int main(int argc, char **argv)
{
	int fd = open("/dev/mem",O_RDWR|O_NDELAY);

	if (fd < 0)
	{
		printf("open /dev/mem error!\n");
		return -1;
	}

	void *map_base = mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,base);
	if (map_base == MAP_FAILED)
		return -1;

	printf("%x \n",*(volatile unsigned int*)(map_base));

	close(fd);
	munmap(map_base,MAP_SIZE);

	return 0;
}*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char text[] = "99";
    char *msg = NULL;
    unsigned long s3 = 13;
    unsigned long s4 = 10;
    int len = 4 * 4 + strlen(text) + 1; // 計算長度
    msg = (char*)malloc(len); // 分配記憶體

    if (msg != NULL) {
        snprintf(msg, len, "%c%c%s%c%c", (char)s3, (char)s4, text, (char)s3, (char)s4);
        // 打印 msg 的內容
        printf("msg: ");
        for (int i = 0; i < len; i++) {
            printf("%d %c",i, msg[i]);
            printf("\n");
        }
        printf("\n");

        free(msg); // 釋放記憶體
    } else {
        printf("Memory allocation failed\n");
    }
	
	
	char s3Val[32];
    char s4Val[32];

    snprintf(s3Val, 32, "%lc", s3);
    snprintf(s4Val, 32, "%lc", s4);
	
	printf("s3 %c", s3Val[0]);
	printf("s4 %c", s4Val[0]);
	printf("s3 %c", s3Val[1]);
	printf("s4 %c", s4Val[1]);
len  = ((strlen(s3Val) * 2) + (strlen(s4Val) * 2)) * sizeof(char);
printf("len %d", len);
    return 0;
}


/*int main() {
    FILE *fp;
    char path[1035];

    fp = popen("mpstat | grep -A 1 \"%idle\" | tail -n 1 | awk '{print $NF}'", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    if (fgets(path, sizeof(path), fp) != NULL) {
        printf("CPU Idle Time: %s", path);
    }
    pclose(fp);

    return 0;
}*/
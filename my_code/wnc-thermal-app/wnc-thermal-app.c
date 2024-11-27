/*===========================================================================
  socket_test_app.c

  DESCRIPTION
  Test App to read log from thermal engine local socket.

  Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.

  ===========================================================================*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define UI_LOCALSOCKET_NAME  "THERMALE_UI"
#define UNIX_PATH_MAX 108

int main()
{
	struct sockaddr_un addr;
	char buf;
	int fd, rc;

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(-1);
	}
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	snprintf(addr.sun_path, UNIX_PATH_MAX, " %s", UI_LOCALSOCKET_NAME);
	/* abstract namespace socket starts with NULL char */
	addr.sun_path[0] = '\0';

	if (connect(fd, (struct sockaddr *) &addr,
		    sizeof(sa_family_t) + 1 + strlen(UI_LOCALSOCKET_NAME) ) != 0) {
		printf("connect error on %s - %s\n",
			UI_LOCALSOCKET_NAME, strerror(errno));
		close(fd);
		return -1;
	}
	printf("connect on %s \n", UI_LOCALSOCKET_NAME);
	while (1) {
		while ((rc = read(fd, &buf, 1)) > 0) {
			if (buf == '\0')
				printf("\n");
			else
				printf("%c",buf);
		}
		if (rc == -1) {
			perror("read");
			exit(-1);
		}
	}

	return 0;
}

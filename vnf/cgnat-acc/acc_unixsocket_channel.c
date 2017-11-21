/*
 * acc_unixsocket_channel.c
 *
 *  Created on: Nov 21, 2017
 *      Author: anlang
 */

#ifndef VNF_CGNAT_ACC_ACC_UNIXSOCKET_CHANNEL_C_
#define VNF_CGNAT_ACC_ACC_UNIXSOCKET_CHANNEL_C_

#include <inttypes.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "acc_log.h"

//extern const char* const g_unix_path ;
#define ACC_UNIX_PATH "/var/run/acc_service_path.sock"

int acc_unixsocket_channel_open(void*args) {
	int fd;
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		return -1;
	}

	struct sockaddr_un un;
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	//snprintf(un.sun_path, sizeof(un.sun_path),ACC_UNIX_PATH, (uintptr_t)args);
	strncpy(un.sun_path, ACC_UNIX_PATH, sizeof(un.sun_path) - 1);
	//unlink(un.sun_path);

	//connect server
	if (connect(fd, (struct sockaddr*) &un, sizeof(un)) == -1) {
		ACC_ERROR("cannot connect to the server!\n");
		close(fd);
		return -1;
	}

	return fd;
}

void acc_unixsocket_channel_close(int fd) {
	close(fd);
}

#endif /* VNF_CGNAT_ACC_ACC_UNIXSOCKET_CHANNEL_C_ */

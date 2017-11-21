/*
 * acc_virtio_channel.c
 *
 *  Created on: Nov 21, 2017
 *      Author: anlang
 */

#ifndef VNF_CGNAT_ACC_ACC_VIRTIO_CHANNEL_C_
#define VNF_CGNAT_ACC_ACC_VIRTIO_CHANNEL_C_


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "be_la_log.h"

#define CHANNEL_PATH "/dev/virtio-ports/pv-mon"

int acc_virtio_channel_open(void) {
	int fd = open(CHANNEL_PATH, O_RDWR);
	if (fd < 0) {
		ACC_ERROR("open channel '%s' fail,error=%s!\n", CHANNEL_PATH,strerror(errno));
		return -1;
	}
	return fd;
}

void acc_virtio_channel_close(int fd) {
	close(fd);
}


#endif /* VNF_CGNAT_ACC_ACC_VIRTIO_CHANNEL_C_ */

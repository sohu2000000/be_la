/*
 * acc_virtio_port.h
 *
 *  Created on: Nov 14, 2017
 *      Author: anlang
 */

#ifndef VNF_CGNAT_ACC_ACC_CHANNEL_H_
#define VNF_CGNAT_ACC_ACC_CHANNEL_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "acc_log.h"

#define CHANNEL_PATH "/dev/virtio-ports/pv-mon"

static inline int acc_channel_open(void) {
	int fd = open(CHANNEL_PATH, O_RDWR);
	if (fd < 0) {
		ACC_ERROR("open channel '%s' fail,error=%s!\n", CHANNEL_PATH,strerror(errno));
		return -1;
	}
	return fd;
}

static inline void acc_channel_close(int fd) {
	close(fd);
}

#endif /* VNF_CGNAT_ACC_ACC_CHANNEL_H_ */

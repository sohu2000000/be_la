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

int acc_unixsocket_channel_open(void*args);
void acc_unixsocket_channel_close(int fd);
int acc_virtio_channel_open(void);
void acc_virtio_channel_close(int fd);

//#define ACC_CHANNEL_USE_UNIXSOCKET 1

static inline int acc_channel_open(void) {
#if ACC_CHANNEL_USE_UNIXSOCKET
	return acc_unixsocket_channel_open(NULL);
#else
	return acc_virtio_channel_open();
#endif
}

static inline void acc_channel_close(int fd) {
#if ACC_CHANNEL_USE_UNIXSOCKET
	acc_unixsocket_channel_close(fd);
#else
	acc_virtio_channel_close(fd);
#endif
}

#endif /* VNF_CGNAT_ACC_ACC_CHANNEL_H_ */

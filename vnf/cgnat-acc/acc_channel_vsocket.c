/*
 * acc_channel_vsocket.c
 *
 *  Created on: Oct 27, 2017
 *      Author: anlang
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>
#include <errno.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "be_module.h"
#include "acc_flow.h"
#include "be_la_log.h"
#include "be_acc_msg.h"
#include "acc_vsocket.h"
#include "acc_channel.h"

//建立连接
void* acc_channel_connect(void*args) {
	//int fd = acc_connect_unix_socket(args);
	int fd = acc_channel_open();
	if (fd < 0) {
		return NULL;
	}

	char hostname[64];
	int hostname_len = 0;

	if (gethostname(hostname, 64)) {
		ACC_ERROR("get hostname fail!\n");
		goto CLOSE_SOCKET;

	}
	hostname_len = strlen(hostname);

	struct acc_msg_header hello;
	hello.acc_type = ACC_TYPE_FLOW;
	hello.code = 0;
	hello.msg_type = ACC_UNIX_HELLO;
	hello.total_len = sizeof(hello) + hostname_len;

	struct acc_msg_header replay;

	if (acc_unix_send_recv((void*) ((intptr_t) fd), &hello, hostname,
			hostname_len, &replay, NULL, NULL)) {
		ACC_ERROR("connect to server fail!\n");
		goto CLOSE_SOCKET;
	}

	if (replay.code == ACC_UNIX_SUCCESS && replay.msg_type == ACC_UNIX_HELLO) {
		return (void*) ((intptr_t) fd);
	}

	ACC_ERROR("connect: Connection refused!\n");

	CLOSE_SOCKET: {
		acc_channel_close(fd);
		return NULL;
	}
}

//添加流
error_code acc_channel_add_flows(void*handle, struct acc_flow*flows, int n_flows) {
	struct acc_msg_header header;
	header.acc_type = ACC_TYPE_FLOW;
	header.code = 0;
	header.msg_type = ACC_UNIX_ADD_FLOWS;
	header.total_len = sizeof(struct acc_msg_header)
			+ (sizeof(*flows) * n_flows);

	struct acc_msg_header replay;

	if (acc_unix_send_recv(handle, &header, (char*)flows, sizeof(*flows) * n_flows,
			&replay, NULL, NULL)) {
		ACC_ERROR("send message to server fail!\n");
		return DISCONNECT;
	}

	if (replay.code == ACC_UNIX_SUCCESS && replay.msg_type == ACC_UNIX_ADD_FLOWS) {
		ACC_LOG("add flows success");
		return FLOWS_ACCEPT;
	} else {

		ACC_ERROR("add flows fail!\n");
		return FLOWS_REJECTION;
	}

}

//断开连接
void acc_channel_disconnect(void*handle) {
	struct acc_msg_header bye;
	bye.msg_type = ACC_UNIX_DISCONNECT;
	bye.acc_type = ACC_TYPE_FLOW;
	bye.code = 0;
	bye.total_len = sizeof(bye);

	struct acc_msg_header replay;

	if (acc_unix_send_recv(handle, &bye, NULL, 0, &replay, NULL, NULL)) {
		ACC_ERROR("send message to server fail!\n");
		goto CLOSE_SOCKET;
	}

	if (replay.code == ACC_UNIX_SUCCESS && replay.msg_type == ACC_UNIX_DISCONNECT) {
		ACC_LOG("disconnect success");
	} else {

		ACC_ERROR("connect: Connection refused!\n");
	}

	CLOSE_SOCKET: {
		int fd = (intptr_t) handle;
		acc_channel_close(fd);
	}
}

int acc_vsocket_unix_register()
{
	struct acc_vsocket_type channel_vsocket=
	{
			.type = "virtio-channel",
			.connect = acc_channel_connect,
			.add_flows = acc_channel_add_flows,
			.disconnect = acc_channel_disconnect
	};

	return acc_vsocket_type_register(&channel_vsocket);
}

module_init(1,acc_vsocket_unix_register);

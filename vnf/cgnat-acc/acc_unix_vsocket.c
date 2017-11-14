/*
 * acc_vsocket_unixsocket.c
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

#include "acc_log.h"
#include "acc_unix_common.h"
#include "acc_flow.h"
#include "acc_vsocket.h"
#include "acc_module.h"

//建立连接
void* acc_unix_connect(void*args) {
	int fd = acc_connect_unix_socket(args);
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

	struct acc_unix_msg_header hello;
	hello.type = ACC_UNIX_HELLO;
	hello.total_len = sizeof(hello) + hostname_len;

	struct acc_unix_msg_header replay;

	if (acc_unix_send_recv((void*) ((intptr_t) fd), &hello, hostname,
			hostname_len, &replay, NULL, NULL)) {
		ACC_ERROR("connect to server fail!\n");
		goto CLOSE_SOCKET;
	}

	if (replay.code == ACC_UNIX_SUCCESS && replay.type == ACC_UNIX_HELLO) {
		return (void*) ((intptr_t) fd);
	}

	ACC_ERROR("connect: Connection refused!\n");

	CLOSE_SOCKET: {
		acc_unix_socket_close(fd);
		return NULL;
	}
}

//添加流
error_code acc_unix_add_flows(void*handle, struct acc_flow*flows, int n_flows) {
	struct acc_unix_msg_header header;
	header.type = ACC_UNIX_ADD_FLOWS;
	header.total_len = sizeof(struct acc_unix_msg_header)
			+ (sizeof(*flows) * n_flows);

	struct acc_unix_msg_header replay;

	if (acc_unix_send_recv(handle, &header, (char*)flows, sizeof(*flows) * n_flows,
			&replay, NULL, NULL)) {
		ACC_ERROR("send message to server fail!\n");
		return DISCONNECT;
	}

	if (replay.code == ACC_UNIX_SUCCESS && replay.type == ACC_UNIX_ADD_FLOWS) {
		ACC_LOG("add flows success");
		return FLOWS_ACCEPT;
	} else {

		ACC_ERROR("add flows fail!\n");
		return FLOWS_REJECTION;
	}

}

//断开连接
void acc_unix_disconnect(void*handle) {
	struct acc_unix_msg_header bye;
	bye.type = ACC_UNIX_DISCONNECT;
	bye.total_len = sizeof(bye);

	struct acc_unix_msg_header replay;

	if (acc_unix_send_recv(handle, &bye, NULL, 0, &replay, NULL, NULL)) {
		ACC_ERROR("send message to server fail!\n");
		goto CLOSE_SOCKET;
	}

	if (replay.code == ACC_UNIX_SUCCESS && replay.type == ACC_UNIX_DISCONNECT) {
		ACC_LOG("disconnect success");
	} else {

		ACC_ERROR("connect: Connection refused!\n");
	}

	CLOSE_SOCKET: {
		int fd = (intptr_t) handle;
		acc_unix_socket_close(fd);
	}
}

int acc_vsocket_unix_register()
{
	struct acc_vsocket_type unix_socket=
	{
			.type = "unix-socket",
			.connect = acc_unix_connect,
			.add_flows = acc_unix_add_flows,
			.disconnect = acc_unix_disconnect
	};

	return acc_vsocket_type_register(&unix_socket);
}

module_init(1,acc_vsocket_unix_register);

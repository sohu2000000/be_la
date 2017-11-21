/*
 * acc_unix_msg.c
 *
 *  Created on: Oct 24, 2017
 *      Author: anlang
 */



#include <assert.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "be_la_log.h"
#include "acc_flow.h"
#include "be_module.h"
#include "be_acc_context.h"
#include "be_acc_msg.h"
#include "acc_rule_be.h"
//#include "event2/event.h"
//#include "acc_unix_server.h"


typedef int (*message_handle_t)(struct acc_context*state, char*message, int len,
		char*out_msg, int* out_len);

static message_handle_t s_msg_handle[ACC_UNIX_MAX_TYPE] = { 0, };

int acc_unix_mssage_register(int type, message_handle_t handle) {
	if (!s_msg_handle[type]) {
		s_msg_handle[type] = handle;
		return 0;
	} else {
		return 1;
	}

}

int be_acc_message_process(struct acc_context*context, int type, char*message,
		int len, char*out_msg, int* out_len) {
	return s_msg_handle[type](context, message, len, out_msg, out_len);
}

int hello_message_handle(struct acc_context*state, char*message, int len,
		char*out_msg, int* out_len) {
	ACC_DEBUG("recv hello message\n");
	if (len > sizeof(state->hostname)) {
		return -1;
	}

	strncpy(state->hostname, message, len);
	state->hostname[len] = '\0';
	strcpy(state->bridge, "br0");
	state->hostid = 0XDEAD;
	state->be = acc_get_first_rule_be();

	*out_len = 0;

	return 0;
}

int add_flows_message_handle(struct acc_context*state, char*message, int len,
		char*out_msg, int* out_len) {
	ACC_DEBUG("recv add-flows message\n");
	//set flow rule
	int ret;
	struct acc_flow*flows = (struct acc_flow*) message;
	int n_flows = len / sizeof(*flows);
	if (n_flows * sizeof(*flows) != len) {
		ACC_ERROR("message length error!\n");
		return -1;
	}

	ret = state->be->acc_add_rules(state, flows, n_flows);
	*out_len = 0;
	return ret;
}

int disconnect_message_handle(struct acc_context*state, char*message, int len,
		char*out_msg, int* out_len) {
	ACC_DEBUG("recv disconnect message\n");
	state->be->acc_flush_rules(state);
	acc_context_destory(state);
	*out_len = 0;
	return 0;
}

int acc_unix_server_msg_init(void) {
	memset(s_msg_handle, 0, sizeof(s_msg_handle));
	s_msg_handle[ACC_UNIX_HELLO] = hello_message_handle;
	s_msg_handle[ACC_UNIX_ADD_FLOWS] = add_flows_message_handle;
	s_msg_handle[ACC_UNIX_DISCONNECT] = disconnect_message_handle;
	return 0;
}

module_init(2,acc_unix_server_msg_init);

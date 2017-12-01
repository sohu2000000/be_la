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
#include "be_acc_card.h"
//#include "event2/event.h"
//#include "acc_unix_server.h"


typedef int (*message_handle_t)(struct be_acc_card_*card, char*message, int len,
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

int be_acc_message_process(struct be_acc_card_*card, int type, char*message,
		int len, char*out_msg, int* out_len) {
	return s_msg_handle[type](card, message, len, out_msg, out_len);
}

int hello_message_handle(struct be_acc_card_*card, char*message, int len,
		char*out_msg, int* out_len) {

	struct acc_context*context;

	ACC_DEBUG("recv hello message\n");
	if (len > sizeof(context->hostname)) {
		return -1;
	}

	if(card->context)
	{
		acc_context_destory(context);
		card->context = NULL;
	}
	//assert(!card->context);
	//assert(!card->plugin);

	card->plugin = be_plugin_match(&card->card);
	if(!card->plugin)
	{
		ACC_ERROR("no plugin match");
		return -1;
	}

	context =  acc_context_alloc();
	if(!context)
	{
		ACC_ERROR("init plugin context fail!\n");
		card->plugin = NULL;
		return -1;
	}

	card->context = context;

	static int context_host_id = 0XDEAD0000;

	strncpy(context->hostname, message, len);
	context->hostname[len] = '\0';
	strcpy(context->bridge, "br0");
	context->hostid = context_host_id++;
	context->be = acc_get_rule_be_by_name("ovs-rule");

	*out_len = 0;

	return 0;
}

int add_flows_message_handle(be_card_inner_t*card, char*message, int len,
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

	struct acc_context*context = card->context;
	assert(context);

	ret = context->be->acc_add_rules(context, flows, n_flows);
	*out_len = 0;
	return ret;
}

int disconnect_message_handle(be_card_inner_t*card, char*message, int len,
		char*out_msg, int* out_len) {
	ACC_DEBUG("recv disconnect message\n");

	struct acc_context*context = card->context;
	assert(context);
	context->be->acc_flush_rules(context);
	acc_context_destory(context);

	card->plugin = NULL;
	card->context = NULL;
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

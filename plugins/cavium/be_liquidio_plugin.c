/*
 * be_netronome_plugin.c
 *
 *  Created on: Nov 21, 2017
 *      Author: anlang
 */

#include <stdint.h>
#include <assert.h>
#include <sys/uio.h>
#include <poll.h>
#include <errno.h>
#include <stdlib.h>

#include "be_acc_context.h"
#include "be_la_log.h"
#include "be_module.h"
#include "be_plugin.h"
#include "be_acc_msg.h"
#include "be_acc_card.h"

int acc_liquidio_proccess(struct be_acc_card_ *card,void * inbuf, uint32_t in_len, void * outbuf,
		int32_t * out_len) {


	struct acc_msg_header* header = (struct acc_msg_header*)inbuf;
	assert(header->total_len == in_len);
	return be_acc_message_process(card, header->msg_type, header->data,
			header->total_len - sizeof(*header),outbuf, out_len);
}

int acc_liquidio_driver_init(void)
{
	return 0;
}

int acc_liquidio_driver_destroy(void)
{
	return 0;
}

void* acc_liquidio_context_init(struct be_acc_card_*card)
{
	struct acc_context* context ;
	struct ssh_remote_execute*remote;

	remote = (struct ssh_remote_execute*)malloc(sizeof(*remote));
	if(!remote)
	{
		return NULL;
	}

	memset(remote,0,sizeof(*remote));

	remote->password="liquidio";
	remote->port=22;
	remote->remote_ip="169.254.1.1";
	remote->username="root";
    remote->env="PATH=\"$PATH:/usr/ovs/bin/\";";

	context = acc_context_alloc();
	if(!context)
	{
		free(remote);
		return NULL;
	}
	context->ssh_remote = remote;
	return context;

}

int acc_liquidio_context_destroy(struct be_acc_card_*card)
{
	struct acc_context* context = card->context;

	assert(context->ssh_remote);
	free(context->ssh_remote);
	context->ssh_remote=NULL;

	acc_context_destory(card->context);
	return 0;
}

static acc_plugin_t s_liquidio_plugin =
{
		.init = acc_liquidio_driver_init,
		.pfunc = acc_liquidio_proccess,
		.destroy = acc_liquidio_driver_destroy,
		.context_init = acc_liquidio_context_init,
		.context_destroy = acc_liquidio_context_destroy,
		.name = "Cavium LiquidIO II plugin",
		.vendor = VENDOR_CAVIUM,
		.model = LIQUIDIO_II,
		.acc_type = ACC_TYPE_FLOW,
};

module_init(10, be_plugin_register, &s_liquidio_plugin);

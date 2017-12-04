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

#include "be_acc_context.h"
#include "be_la_log.h"
#include "be_module.h"
#include "be_plugin.h"
#include "be_acc_msg.h"
#include "be_acc_card.h"

int acc_netronome_proccess(struct be_acc_card_ *card,void * inbuf, uint32_t in_len, void * outbuf,
		int32_t * out_len) {


	struct acc_msg_header* header = (struct acc_msg_header*)inbuf;
	assert(header->total_len == in_len);
	return be_acc_message_process(card, header->msg_type, header->data,
			header->total_len - sizeof(*header),outbuf, out_len);
}

int acc_netronome_driver_init(void)
{
	return 0;
}

int acc_netronome_driver_destroy(void)
{
	return 0;
}

void* acc_netronome_context_init(struct be_acc_card_*card)
{
	return acc_context_alloc();
}

int acc_netronome_context_destroy(struct be_acc_card_*card)
{
	acc_context_destory(card->context);
	return 0;
}

static acc_plugin_t s_netronome_plugin =
{
		.init = acc_netronome_driver_init,
		.pfunc = acc_netronome_proccess,
		.destroy = acc_netronome_driver_destroy,
		.context_init = acc_netronome_context_init,
		.context_destroy = acc_netronome_context_destroy,
		.name = "netronome plugin",
		.vendor = VENDOR_NETRONOME,
		.model =
				AGILIO_ISA_4000,
		.acc_type = ACC_TYPE_FLOW | ACC_TYPE_COMPRESS,
};

module_init(10, be_plugin_register, &s_netronome_plugin);

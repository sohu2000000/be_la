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

int acc_netronome_proccess(void * inbuf, uint32_t in_len, void * outbuf,
		int32_t * out_len) {


	struct acc_msg_header* header = (struct acc_msg_header*)inbuf;
	assert(header->total_len == in_len);
	return be_acc_message_process(NULL, header->type, header->data,
			header->total_len - sizeof(*header),outbuf, out_len);
}

static acc_plugin_t s_netronome_plugin = { .pfunc = acc_netronome_proccess,
		.name = "netronome plugin", .vendor = VENDOR_NETRONOME, .model =
				AGILIO_ISA_4000, };

module_init(10, be_plugin_register, &s_netronome_plugin);

/*
 * be_netronome_plugin.c
 *
 *  Created on: Nov 21, 2017
 *      Author: anlang
 */

#include "be_plugin.h"
#include "be_module.h"

int acc_netronome_proccess(void * inbuf, unsigned int in_len, void * outbuf,
		unsigned int * out_len) {

#if 0
	acc_proc_msg_t * msg = NULL, *out_msg = NULL;

	if (NULL == inbuf || NULL == outbuf || NULL == out_len_ptr) {
		BE_LA_LOG("agilio_isa_4000_proccess\n");
		return NULL;
	}

	msg = (acc_proc_msg_t*) inbuf;
	out_msg = (acc_proc_msg_t*) outbuf;

	/*input process*/
	BE_LA_LOG("agilio_isa_4000_proccess acc_type: 0x%x, buffer: %s\n",
			msg->acc_type, msg->buffer);

	/*output feedback */
	strcpy(out_msg->buffer,
			"I am Hypervisor, agilio_isa_4000_proccess result\n");
	out_msg->acc_type = msg->acc_type;
	*out_len_ptr = sizeof(acc_proc_msg_t);
#endif

	return -1;
}

static acc_plugin_t s_netronome_plugin = { .pfunc = acc_netronome_proccess,
		.name = "netronome plugin", .vendor = VENDOR_NETRONOME, .model =
				AGILIO_ISA_4000, };

module_init(10, be_plugin_register, &s_netronome_plugin);

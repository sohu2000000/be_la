/*
 * be_intel_qat_plugin.c
 *
 *  Created on: Dec 26, 2017
 *      Author: Lei Sun
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

int acc_intel_qat_proccess(struct be_acc_card_ *card,void * inbuf, uint32_t in_len, void * outbuf,
		int32_t * out_len) {


	struct acc_msg_header* header = (struct acc_msg_header*)inbuf;
	assert(header->total_len == in_len);
	return be_acc_message_process(card, header->msg_type, header->data,
			header->total_len - sizeof(*header),outbuf, out_len);
}

int acc_intel_qat_driver_init(void)
{
	return 0;
}

int acc_intel_qat_driver_destroy(void)
{
	return 0;
}

void* acc_intel_qat_context_init(struct be_acc_card_*card)
{
	return acc_context_alloc();
}

int acc_intel_qat_context_destroy(struct be_acc_card_*card)
{
	acc_context_destory(card->context);
	return 0;
}

static acc_plugin_t s_intel_qat_plugin =
{
		.init = acc_intel_qat_driver_init,
		.pfunc = acc_intel_qat_proccess,
		.destroy = acc_intel_qat_driver_destroy,
		.context_init = acc_intel_qat_context_init,
		.context_destroy = acc_intel_qat_context_destroy,
		.name = "Intel QAT plugin",
		.vendor = VENDOR_INTEL,
		.model = QAT_DH8950,
		.acc_type = ACC_TYPE_CRYPTO | ACC_TYPE_DECRYPTO | ACC_TYPE_COMPRESS | ACC_TYPE_UNCOMPRESS,
};

module_init(10, be_plugin_register, &s_intel_qat_plugin);

/*****************************************************************
 * Copyright (C) 2017 Lenovo Co.,Ltd.*
 ******************************************************************
 *
 * FILENAME:
 *   be_la_cmd.c
 *
 * DESCRIPTION:
 *
 * AUTHOR:
 *   Feng Liu 626987
 *
 * CREATED DATE:
 *   28/10/2017
 * REVISION:
 *   1.0
 *
 * MODIFICATION HISTORY
 * --------------------
 * $Log:$
 *   Feng Liu 28/10/2017  Creation
 *****************************************************************/

#include <assert.h>

#include "be_acc_card.h"
#include "be_la_log.h"
#include "be_channel.h"

int be_la_cmd_process(cmd_hdr_t * cmd, void*arg) {
	assert(cmd);

	switch (cmd->cmd_type) {
	case CMD_TYPE_REGISTER:
		if (be_acc_card_reg(&cmd->card)) {
			return -1;
		}

		/*temp code*/
		be_acc_cards_dump();

		if (vm_channel_open(&cmd->card, arg)) {
			BE_LA_ERROR("connect to vm %s,path %s failed\n", cmd->card.vm_uuid,cmd->card.vmpath);
			return -1;
		}
		return 0;
	case CMD_TYPE_UNREGISTER:
		return be_acc_card_unreg(&cmd->card);
	default:
		BE_LA_ERROR("unsupported command type\n");
		return -1;

	}
}


/*
 * be_acc_card.c
 *
 *  Created on: Nov 22, 2017
 *      Author: anlang
 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "be_la_log.h"
#include "be_la_vendor.h"
#include "be_acc_card.h"

#define ACC_CARD_MAX_NUM      64

struct acc_cards_table {
	acc_card_t * acc_cards[ACC_CARD_MAX_NUM];
	int reg_num;
};

struct acc_cards_table g_acc_cards;


int be_la_get_acc_card(char*vmpath) {
	assert(vmpath);

	int i = 0;
	for (i = 0; i < ACC_CARD_MAX_NUM; ++i) {
		if (g_acc_cards.acc_cards[i] && strcmp(g_acc_cards.acc_cards[i]->vmpath, vmpath) == 0) {
			return i;
		}
	}

	return -1;
}

acc_card_t* be_la_get_acc_cardptr(char* vmpath)
{
	int idx;
	idx = be_la_get_acc_card(vmpath);
	if(idx >=0)
	{
		return g_acc_cards.acc_cards[idx];
	}
	return NULL;
}

int be_acc_card_reg(acc_card_t* reginfo) {

	int i = 0;

	assert(reginfo);
	assert(reginfo->vm_uuid[0]);
	assert(reginfo->vmpath[0]);

	if (be_la_get_acc_card(reginfo->vmpath) >= 0) {
		BE_LA_ERROR("acc card is exists");
		return -1;
	}

	for (i = 0; i < ACC_CARD_MAX_NUM; i++) {
		if (!g_acc_cards.acc_cards[i]) {
			acc_card_t *card = (acc_card_t *) calloc(1,sizeof(be_card_inner_t));
			if (!card) {
				BE_LA_ERROR("acc card malloc failed\n");
				return -1;
			}

			memcpy(card, reginfo, sizeof(*reginfo));
			g_acc_cards.acc_cards[i] = card;
			g_acc_cards.reg_num++;
			return 0;
		}
	}

	return -1; //is_full
}

int be_acc_card_unreg(acc_card_t* unreginfo)
{
	//TODO
	*(int*)0=0;
	return 0;
}


void be_acc_cards_dump(void) {
	int i = 0;

	BE_LA_LOG("DUMP ACC Cards\n");
	for (i = 0; i < ACC_CARD_MAX_NUM; i++) {
		if (g_acc_cards.acc_cards[i]) {
			acc_card_t * card = g_acc_cards.acc_cards[i];
			BE_LA_LOG(
					"\tacc_cards[%d]: vendor = %u, model = %u, acc_type = 0x%x vmuuid = %s, vmpath = %s\n",
					i, card->vendor, card->model, card->acc_type, card->vm_uuid,
					card->vmpath);
		}
	}
}

int be_la_acc_cards_init(void) {

	memset(&g_acc_cards, 0x0, sizeof(g_acc_cards));
	g_acc_cards.reg_num = 0;

	return 0;
}

void be_la_acc_cards_exit(void) {

	int i = 0;

	for (i = 0; i < ACC_CARD_MAX_NUM; i++) {
		if (g_acc_cards.acc_cards[i]) {
			free(g_acc_cards.acc_cards[i]);
		}
	}

	memset(&g_acc_cards, 0x0, sizeof(g_acc_cards));
	g_acc_cards.reg_num = 0;
}

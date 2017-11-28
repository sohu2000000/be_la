/*
 * be_acc_card.h
 *
 *  Created on: Nov 22, 2017
 *      Author: anlang
 */

#ifndef BE_LA_BE_ACC_CARD_H_
#define BE_LA_BE_ACC_CARD_H_

#include "be_la_cmd.h"
#include "be_plugin.h"


typedef struct be_acc_card_{
	acc_card_t card;//must be first
	int fd;
	void* context;
	void* read_event;
	acc_plugin_t* plugin;
}be_card_inner_t;

int be_la_acc_cards_init(void);
int be_acc_card_reg(acc_card_t * reginfo);
int be_acc_card_unreg(acc_card_t* unreginfo);
void be_acc_cards_dump(void);


#endif /* BE_LA_BE_ACC_CARD_H_ */

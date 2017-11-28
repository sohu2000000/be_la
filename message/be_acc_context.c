/*
 * be_acc_context.c
 *
 *  Created on: Nov 21, 2017
 *      Author: anlang
 */

#include <stdlib.h>

#include "be_acc_context.h"
#include "be_la_log.h"

struct acc_context* acc_context_alloc() {
	struct acc_context*state = malloc(sizeof(*state));
	if (!state) {
		ACC_ERROR("alloc fd_state fail!\n");
		return NULL;
	}
	return state;
}

void acc_context_destory(struct acc_context*context) {
		free(context);

}

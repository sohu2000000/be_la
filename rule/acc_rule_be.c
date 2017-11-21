/*
 * acc_rule_be.c
 *
 *  Created on: Oct 26, 2017
 *      Author: anlang
 */



#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

//#include "acc_unix_msg.h"
#include "acc_rule_be.h"
#include "acc_flow.h"


#define ACC_RULE_BE_MAX_SIZE 32
static struct acc_rule_be s_acc_rule_be[ACC_RULE_BE_MAX_SIZE];

struct acc_rule_be* acc_get_first_rule_be() {
	if (s_acc_rule_be[0].name[0] != '\0') {
		return &s_acc_rule_be[0];
	}
	return NULL;
}

struct acc_rule_be* _get_rule_be_by_name(char*name, int *loc) {
	int i;
	for (i = 0; i < ACC_RULE_BE_MAX_SIZE; ++i) {
		if (s_acc_rule_be[i].name[0] != '\0') {
			if (!strncmp(name, s_acc_rule_be[i].name,
					sizeof(s_acc_rule_be[i].name))) {
				return &s_acc_rule_be[i];
			}
		}
		else
		{
			break;
		}
	}

	if (loc) {
		*loc = i;
	}
	return NULL;
}

struct acc_rule_be* acc_get_rule_be_by_name(char*name) {
	return _get_rule_be_by_name(name, NULL);
}


int acc_rule_be_register(struct acc_rule_be*be) {
	if (!be || be->name[0] == '\0') {
		return -1; //invalid parameter
	}

	int loc;
	if (_get_rule_be_by_name(be->name, &loc)) {
		return -1; //exist
	}

	if (loc >= ACC_RULE_BE_MAX_SIZE) {
		return -1; //full
	}

	memcpy(&s_acc_rule_be[loc], be, sizeof(*be));
	return 0;
}

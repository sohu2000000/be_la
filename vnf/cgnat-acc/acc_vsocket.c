/*
 * acc_vsocket.c
 *
 *  Created on: Oct 27, 2017
 *      Author: anlang
 */

#define ACC_CLIENT_MAX_SIZE 32

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "acc_flow.h"
#include "acc_vsocket.h"

static struct acc_vsocket_type s_acc_vsocket_types[ACC_CLIENT_MAX_SIZE];

static inline struct acc_vsocket_type* _acc_vsocket_type_get(int idx) {
	return &s_acc_vsocket_types[idx];
}

static inline int _acc_vsocket_get_by_name(const char*name) {
	int i;
	for (i = 0; i < ACC_CLIENT_MAX_SIZE; ++i) {
		if (s_acc_vsocket_types[i].type[0]
				&& strcmp(name, s_acc_vsocket_types[i].type)) {
			return i;
		}
	}

	return -1;
}

struct acc_vsocket_type* acc_get_first_vsocket_type() {
	if (s_acc_vsocket_types[0].type[0]) {
		return &s_acc_vsocket_types[0];
	}
	return NULL;
}

static inline int _acc_vsocket_type_get_last_idx() {
	int i;
	for (i = 0; i < ACC_CLIENT_MAX_SIZE; ++i) {
		if (!s_acc_vsocket_types[i].type[0]) {
			break;
		}
	}

	if (i < ACC_CLIENT_MAX_SIZE) {
		//return _acc_vsocket_type_get(i);
		return i;
	}

	return -1;
}

struct acc_vsocket_type* acc_get_vsocket_type_by_name(const char*name) {
	int idx = _acc_vsocket_get_by_name(name);
	if (idx < 0) {
		return NULL;
	}

	return _acc_vsocket_type_get(idx);
}

static inline int _acc_vsocket_type_add(struct acc_vsocket_type* vsocket_type) {
	int idx = _acc_vsocket_type_get_last_idx();
	if (idx < 0) {
		return -1;
	}

	memcpy(&s_acc_vsocket_types[idx], vsocket_type, sizeof(*vsocket_type));
	return 0;
}

int acc_vsocket_type_register(struct acc_vsocket_type* vsocket_type) {
	struct acc_vsocket_type* type;
	type = acc_get_vsocket_type_by_name(vsocket_type->type);
	if (type) {
		return -1;
	}

	return _acc_vsocket_type_add(vsocket_type);
}


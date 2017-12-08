/*
 * acc_client.c
 *
 *  Created on: Oct 27, 2017
 *      Author: anlang
 */
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "acc_flow.h"
#include "acc_vsocket.h"
#include "acc_client.h"


#define ACC_CLIENT_MAX_SIZE 32

extern int acc_vsocket_unix_register();

static struct acc_client s_acc_client[ACC_CLIENT_MAX_SIZE];

static inline struct acc_client* _acc_client_get(int idx) {
	return &s_acc_client[idx];
}

static inline int _acc_client_get_by_id(const void*id) {
	int i;
	for (i = 0; i < ACC_CLIENT_MAX_SIZE && s_acc_client[i].id; ++i) {
		if (id == s_acc_client[i].id)
			return i;
	}
	return -1;
}

static inline int _acc_client_get_last_idx() {
	int i;
	for (i = 0; i < ACC_CLIENT_MAX_SIZE && s_acc_client[i].id; ++i) {
		if (!s_acc_client[i].id)
			break;
	}

	if (i < ACC_CLIENT_MAX_SIZE) {
		return i;
	}

	return -1;
}

static inline struct acc_client* acc_client_get(const void* id) {
	int idx = _acc_client_get_by_id(id);
	if (idx >= 0) {
		return _acc_client_get(idx);
	}
	return NULL;
}

static inline struct acc_client* acc_client_add(const void* id,
		const char*socket_type) {
	struct acc_client* new_client = NULL;
	int idx = _acc_client_get_last_idx();
	if (idx >= 0) {
		struct acc_vsocket_type* socket;
		if (socket_type) {
			socket = acc_get_vsocket_type_by_name(socket_type);
		} else {
			acc_vsocket_unix_register();
			socket = acc_get_first_vsocket_type();
		}

		if (!socket) {
			return NULL;
		}

		new_client = _acc_client_get(idx);
		new_client->id = (void*)id;
		new_client->handle = NULL;
		new_client->socket = socket;
	}
	return new_client;
}

struct acc_client* acc_client_register(const void*id) {
	struct acc_client* client;
	client = acc_client_get(id);
	if (client) {
		return client;
	}

	return acc_client_add(id,NULL);
}

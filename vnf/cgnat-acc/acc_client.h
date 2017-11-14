/*
 * acc_client.h
 *
 *  Created on: Oct 27, 2017
 *      Author: anlang
 */

#ifndef SRC_AGENT_ACC_AGENT_ACC_CLIENT_H_
#define SRC_AGENT_ACC_AGENT_ACC_CLIENT_H_

struct acc_client {
	void* id;
	void* handle;
	struct acc_vsocket_type* socket;
};

struct acc_client* acc_client_register(const void*);

static inline int acc_client_is_connected(struct acc_client*client) {
	return client->handle != NULL;
}

static inline int acc_client_connect(struct acc_client*client) {
	client->handle = client->socket->connect(NULL);
	return (intptr_t)client->handle;
}

static inline error_code acc_client_add_flows(struct acc_client*client,
		struct acc_flow*acc_flows, uint32_t len) {
	assert(client);
	assert(acc_client_is_connected(client));
	return client->socket->add_flows(client->handle, acc_flows, len);
}

#endif /* SRC_AGENT_ACC_AGENT_ACC_CLIENT_H_ */

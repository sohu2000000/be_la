/*
 * acc_vscoket.h
 *
 *  Created on: Oct 27, 2017
 *      Author: anlang
 */

struct acc_vsocket_type {
	char type[32];
	void* (*connect)(void*args);
	error_code (*add_flows)(void*handle, struct acc_flow*flows, int n_flows);
	void (*disconnect)(void*handle);
};


int acc_vsocket_type_register(struct acc_vsocket_type* vsocket_type);
struct acc_vsocket_type* acc_get_first_vsocket_type();
struct acc_vsocket_type* acc_get_vsocket_type_by_name(const char*name);

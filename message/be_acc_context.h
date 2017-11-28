/*
 * be_context.h
 *
 *  Created on: Nov 21, 2017
 *      Author: anlang
 */

#ifndef MESSAGE_BE_CONTEXT_H_
#define MESSAGE_BE_CONTEXT_H_

struct acc_context {
	char hostname[256];
	char bridge[256];
	int hostid;//use for cookie
	struct acc_rule_be* be;
	//char buffer[MAX_LINE];
	//size_t buffer_used;
	//size_t n_written;
	//size_t write_upto;
	struct event*read_event;
	//struct event*write_event;
};

struct acc_context* acc_context_alloc();
void acc_context_destory(struct acc_context*context);

#endif /* MESSAGE_BE_CONTEXT_H_ */

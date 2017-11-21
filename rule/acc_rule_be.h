/*
 * acc_rule.h
 *
 *  Created on: Oct 26, 2017
 *      Author: anlang
 */

struct acc_rule_be {
	char name[32];
	int (*acc_add_rules)(struct acc_context*state, struct acc_flow*flows,
			int n_flows);
	int (*acc_remove_rules)(struct acc_context*state, struct acc_flow*flows,
			int n_flows);
	int (*acc_flush_rules)(struct acc_context*state);
};

int acc_rule_be_register(struct acc_rule_be*be);
struct acc_rule_be* acc_get_rule_be_by_name(char*name);
struct acc_rule_be* acc_get_first_rule_be();

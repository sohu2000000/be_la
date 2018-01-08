/*
 * acc_ovs_rule.c
 *
 *  Created on: Oct 25, 2017
 *      Author: anlang
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/uio.h>
#include <assert.h>
#include <poll.h>

#include "be_acc_context.h"
#include "acc_flow.h"
#include "acc_rule_be.h"
#include "be_module.h"
#include "be_la_log.h"
#include "be_acc_msg.h"
#include "ssh_remote_exec.h"

#define GET_BYTE(value,idx) value[idx]

#define GET_IP_ARGS(value) GET_BYTE(value, 0), GET_BYTE(value, 1),\
		GET_BYTE(value, 2), GET_BYTE(value, 3)

#define GET_MAC_ARGS(value) GET_BYTE(value, 0), GET_BYTE(value, 1),\
		GET_BYTE(value, 2), GET_BYTE(value, 3),GET_BYTE(value, 4), \
		GET_BYTE(value, 5)

static inline char* _print_port(uint16_t port, char*buffer) {
	sprintf(buffer, "%d", port);
	return buffer;
}
static inline char* _print_ip(uint32_t ip, char*buffer) {
	uint8_t* ipaddr = (uint8_t*) &ip;
	sprintf(buffer, "%d.%d.%d.%d", GET_IP_ARGS(ipaddr));
	return buffer;
}

static inline char* _print_mac(uint8_t mac[6], char*buffer) {
	sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", GET_MAC_ARGS(mac));
	return buffer;
}

char* print_ip1(uint32_t ip) {
	static char buffer[16]; //strlen("255.255.255.255") == 15
	return _print_ip(ip, buffer);
}

char* print_ip2(uint32_t ip) {
	static char buffer[16]; //strlen("255.255.255.255") == 15
	return _print_ip(ip, buffer);
}

char* print_mac1(uint8_t mac[6]) {
	static char buffer[18]; //strlen("AA:BB:CC:DD:EE:FF") == 17
	return _print_mac(mac, buffer);
}

char* print_mac2(uint8_t mac[6]) {
	static char buffer[18]; //strlen("AA:BB:CC:DD:EE:FF") == 17
	return _print_mac(mac, buffer);
}

char* print_port1(uint16_t port) {
	static char buffer[8]; //"65535"
	return _print_port(port, buffer);
}

char* print_port2(uint16_t port) {
	static char buffer[8]; //"65535"
	return _print_port(port, buffer);
}

struct acc_ofctl_rule {
	uint32_t cookie;
	uint32_t priority;
	uint32_t table;
	char match[1024];
	char action[1024];
};
static inline struct acc_ofctl_rule* acc_ovs_ofctl_rule_alloc(int n_flows) {
	struct acc_ofctl_rule *rules = calloc(n_flows,
			sizeof(struct acc_ofctl_rule));
	return rules;
}

static inline void acc_ovs_alloc_ofctl_rules(struct acc_ofctl_rule*rules) {
	free(rules);
}

static inline int acc_ovs_build_ofctl_rule(struct acc_ofctl_rule*rule,
		struct acc_context*state, struct acc_flow*flow) {
	int use_len = 0;
	rule->cookie = state->hostid;
	rule->priority = 68;
	rule->table = 0;

#define _PK(flow,name,key) ACC_MATCH_IS_FIELD_MASK_SET(flow->mask,name)? key:""
#define _PV(flow,name,key,func) ACC_MATCH_IS_FIELD_MASK_SET(flow->mask,name)? func(flow->match.name):""
#define _FARG(name,key,func) _PK(flow,name,key) ,\
		_PV(flow,name,key,func)

	int ret = snprintf(rule->match, sizeof(rule->match),
			"%s%s %s%s ip %s%s %s%s", _FARG(srcmac, " eth_src=", print_mac1),
			_FARG(dstmac, " eth_dst=", print_mac2),
			_FARG(src_ip, " nw_src=", print_ip1),
			_FARG(dst_ip, " nw_dst=", print_ip2));
	if (ret < 0 || ret >= sizeof(rule->match)) {
		ACC_ERROR("build rule fail(ethnet and ipaddr)!\n");
		return -1;
	}

	use_len = ret;
	switch (flow->match.protocol) {
	//case 1://protocol
	//	snprintf(((char*)rule->match)+ret,"icmp %s %s",_FARG());
	//	break;
	case 6:	//tcp
		ret = snprintf(((char*) rule->match) + use_len,
				sizeof(rule->match) - use_len, " tcp %s%s %s%s",
				_FARG(src_port, " tcp_src=", print_port1),
				_FARG(dst_port, " tcp_dst=", print_port2));
		break;
	case 17:	//udp
		ret = snprintf(((char*) rule->match) + use_len,
				sizeof(rule->match) - use_len, " udp %s%s %s%s",
				_FARG(src_port, " udp_src=", print_port1),
				_FARG(dst_port, " udp_dst=", print_port2));
		break;
	default:
		ACC_ERROR("no support protocol %d\n", flow->match.protocol)
		;
		return -1;
	}

	if (ret < 0 || ret + use_len >= sizeof(rule->match)) {
		ACC_ERROR("build rule fail(port)!\n");
		return -1;
	}

	use_len += ret;

#undef _PK
#undef _PV
#define _PK(flow,name,key)      ACC_ACTION_IS_FIELD_MASK_SET(flow->action,name)? key:""
#define _PV(flow,name,key,func) ACC_ACTION_IS_FIELD_MASK_SET(flow->action,name)? func(flow->action.u.field_update.name):""
	//build action
	ret = snprintf(rule->action, sizeof(rule->action),
			" actions=%s%s %s%s %s%s  %s%s %s%s %s%s output:normal",
			_FARG(srcmac, " mod_dl_src:", print_mac1),
			_FARG(dstmac, " mod_dl_dst:", print_mac2),
			_FARG(src_ip, " mod_nw_src:", print_ip1),
			_FARG(dst_ip, " mod_nw_dst:", print_ip2),
			_FARG(src_port, " mod_tp_src:", print_port1),
			_FARG(dst_port, " mod_tp_dst:", print_port2));
	if (ret < 0 || ret >= sizeof(rule->action)) {
		ACC_ERROR("build rule fail(ethnet and ipaddr)!\n");
		return -1;
	}

#undef _FARG
#undef _PK
#undef _PV
	return 0;
}

static inline struct acc_ofctl_rule* acc_ovs_make_ofctl_rules(
		struct acc_context*state, struct acc_flow*flows, int n_flows) {
	struct acc_ofctl_rule*rules = acc_ovs_ofctl_rule_alloc(n_flows);
	if (!rules) {
		ACC_ERROR("alloc memory fail!\n");
		return NULL;
	}

	int i;
	for (i = 0; i < n_flows; ++i) {
		if (acc_ovs_build_ofctl_rule(&rules[i], state, &flows[i])) {
			ACC_ERROR("build ofctl rule fail!\n");
			goto FREE_RULES;
		}
	}

	return rules;
	FREE_RULES: {
		acc_ovs_alloc_ofctl_rules(rules);
		return NULL;
	}
}

static inline int execute_openflow_rule(struct acc_context*state, char*rule) {
	//ACC_LOG("execute :%s\n", rule);
	if (state->ssh_remote) {
		struct ssh_remote_execute* remote = state->ssh_remote;
		ACC_LOG("execute: ssh -p %d %s@%s '%s'",remote->username,remote->port,remote->remote_ip,rule);
		if (ssh_remote_exec(remote->remote_ip, remote->username, remote->port,
				remote->password, rule)) {
			ACC_ERROR("execute command line '%s' fail!\n", rule);
			return -1;
		}
		return 0;
	}

	ACC_LOG("execute :%s\n", rule);
	if (system(rule)) {
		ACC_ERROR("execute command line '%s' fail,error=%s!\n", rule,strerror(errno));
		return -1;
	}
	return 0;
}

int acc_ovs_flush_rules(struct acc_context*state) {
	int ret;
	char buffer[4096];
	/**
	 * ovs-ofctl add-flow br0 'cookie=0xdead,priority=300,table=10,eth_dst=00:02:03:04:05:06,
	 ip,nw_src=10.1.2.3,ip_proto=6,tcp_src=33445,
	 actions=mod_dl_src:00:12:13:14:15:16,mod_dl_src:00:22:23:24:25:26,
	 mod_nw_src:10.2.2.3,mod_tp_src:9934,output:normal'
	 */
	ret = snprintf(buffer, sizeof(buffer),
			"ovs-ofctl del-flows %s cookie=0X%X/-1", state->bridge,
			state->hostid);
	if (ret < 0 || ret >= sizeof(buffer)) {
		ACC_ERROR("build flush command line fail!\n");
		return -1;
	}

#if 0
	ACC_LOG("execute :%s\n", buffer);
	if (system(buffer)) {
		ACC_ERROR("execute command line '%s' fail!\n", buffer);
		return -1;
	}
	return 0;
#else
	return execute_openflow_rule(state, buffer);
#endif

}

static inline int acc_ovs_edit_rules(struct acc_context*state,
		struct acc_flow*flows, int n_flows, int is_delete) {
	struct acc_ofctl_rule*rules = acc_ovs_make_ofctl_rules(state, flows,
			n_flows);
	if (!rules) {
		return -1;
	}

	int ret;
	int i;
	for (i = 0; i < n_flows; ++i) {
		char buffer[4096];
		/**
		 * ovs-ofctl add-flow br0 'cookie=0xdead,priority=300,table=10,eth_dst=00:02:03:04:05:06,
		 ip,nw_src=10.1.2.3,ip_proto=6,tcp_src=33445,
		 actions=mod_dl_src:00:12:13:14:15:16,mod_dl_src:00:22:23:24:25:26,
		 mod_nw_src:10.2.2.3,mod_tp_src:9934,output:normal'
		 */
		ret = snprintf(buffer, sizeof(buffer),
				"ovs-ofctl %s %s 'cookie=0X%X priority=%d table=%d %s %s'",
				is_delete ? "del-flow" : "add-flow", state->bridge,
				rules[i].cookie, rules[i].priority, rules[i].table,
				rules[i].match, rules[i].action);
		if (ret < 0 || ret >= sizeof(buffer)) {
			ACC_ERROR("build command line fail!\n");
			goto FREE_OFCTL_RULES;
		}

		if (execute_openflow_rule(state, buffer)) {
			goto FREE_OFCTL_RULES;
		}
	}

	return 0;

	FREE_OFCTL_RULES: {
		acc_ovs_alloc_ofctl_rules(rules);
		if (!is_delete) {
			acc_ovs_flush_rules(state);
		}
		return -1;
	}
}

int acc_ovs_add_rules(struct acc_context*state, struct acc_flow*flows,
		int n_flows) {
	return acc_ovs_edit_rules(state, flows, n_flows, 0);
}

int acc_ovs_remove_rules(struct acc_context*state, struct acc_flow*flows,
		int n_flows) {
	return acc_ovs_edit_rules(state, flows, n_flows, 1);
}

static int acc_ovs_rule_be_register() {

	struct acc_rule_be acc_rule_ovs = { .name = "ovs-rule", .acc_add_rules =
			acc_ovs_add_rules, .acc_remove_rules = acc_ovs_remove_rules,
			.acc_flush_rules = acc_ovs_flush_rules };

	return acc_rule_be_register(&acc_rule_ovs);

}

module_init(1, acc_ovs_rule_be_register);


#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

//#include "app.h"
//#include "pipeline_common_be.h"
#include "acc_flow.h"
#include "acc_vsocket.h"
#include "acc_client.h"
#include "cgnapt_accelerate.h"

static inline void acc_mac_copy(uint8_t to[6], uint8_t from[6]) {
	memcpy(to, from, 6);
}

static inline void acc_action_srcmac_set(struct acc_action* action,
		uint8_t srcmac[6]) {
	acc_mac_copy(action->u.field_update.srcmac, srcmac);
	ACC_ACTION_FIELD_MASK_SET((*action), srcmac);
}
static inline void acc_action_dstmac_set(struct acc_action* action,
		uint8_t dstmac[6]) {
	acc_mac_copy(action->u.field_update.dstmac, dstmac);
	ACC_ACTION_FIELD_MASK_SET((*action), dstmac);
}

void cgnat_accelerate_make_flow(struct acc_flow*cgnat_flow, uint32_t src_ip,
		uint16_t src_port, uint8_t protocol, uint32_t xlate_src_ip,
		uint16_t xlate_src_port, uint8_t xlate_srcmac[6],
		uint8_t xlate_dstmac[6]) {
	assert(cgnat_flow);
	memset(cgnat_flow, 0, sizeof(*cgnat_flow));

//设置nat的match及mask字段
//cgnat_flow->match.srcmac
//cgnat_flow->mask.srcmac

//cgnat_flow->match.dstmac
//cgnat_flow->mask.dstmac

	cgnat_flow->match.src_ip = src_ip;
	cgnat_flow->mask.src_ip = 0XFFFFFFFF;

//cgnat_flow->match.dst_ip
//cgnat_flow->mask.dst_ip

	cgnat_flow->match.src_port = src_port;
	cgnat_flow->mask.src_port = 0XFFFF;

//cgnat_flow->match.dst_port;
//cgnat_flow->mask.dst_port;

	cgnat_flow->match.protocol = protocol;
	cgnat_flow->mask.protocol = 0XFF;

//set action:
	acc_action_srcmac_set(&(cgnat_flow->action), xlate_srcmac);
	acc_action_dstmac_set(&(cgnat_flow->action), xlate_srcmac);
	ACC_ACTION_FIELD_UPDATE_SET(cgnat_flow->action, src_ip, xlate_src_ip);
	ACC_ACTION_FIELD_UPDATE_SET(cgnat_flow->action, src_port, xlate_src_port);
}



/**
 * @id 用于唯一标识一个线程
 * ＠src_ip 报文的源ip
 * @src_port 报文的运输层源port
 * ＠protocol 运输层协议
 * @xlate_src_ip 转换后源ip
 * @xlate_src_port 转换后源port
 * @xlate_srcmac 转换后报文源mac地址
 * @xlate_dstmac 转换后报文目的mac地址
 * ＠return 0 成功下发
 * @return -1 下发失败
 */
int cgnat_accelerate(void*id, uint32_t src_ip, uint16_t src_port,
		uint8_t protocol, uint32_t xlate_src_ip, uint16_t xlate_src_port,
		uint8_t xlate_srcmac[6], uint8_t xlate_dstmac[6]) {

	error_code ret = DISCONNECT;

	assert(id);

	struct acc_flow cgnat_flow;
	struct acc_client*client = acc_client_register(id);
	assert(client);

	cgnat_accelerate_make_flow(&cgnat_flow, src_ip, src_port, protocol,
			xlate_src_ip, xlate_src_port, xlate_srcmac, xlate_dstmac);
	if (acc_client_is_connected(client)) {
		ret = acc_client_add_flows(client, &cgnat_flow, 1);
	}

	if (ret == DISCONNECT) {
		if (!acc_client_connect(client)) {
			return DISCONNECT;
		}
		ret = acc_client_add_flows(client, &cgnat_flow, 1);
	}

	return ret == FLOWS_ACCEPT;
}

#if FOR_TEST
int main(int argc,char**argv)
{
	uint8_t mac1[6]="abcdef",mac2[6]="12345";
	printf("%d\n",cgnat_accelerate((void*)1,3,4,6,6,7,mac1,mac2));
	printf("%d\n",cgnat_accelerate((void*)1,3,4,6,6,7,mac1,mac2));
	return 0;
}
#endif

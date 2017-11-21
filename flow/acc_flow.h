/*
 * acc_flow.h
 *
 *  Created on: Oct 26, 2017
 *      Author: anlang
 */

#ifndef SRC_FLOW_ACC_FLOW_H_
#define SRC_FLOW_ACC_FLOW_H_

struct acc_match {
	uint8_t srcmac[6];
	uint8_t dstmac[6];
	uint32_t src_ip;
	uint32_t dst_ip;
	uint16_t src_port;
	uint16_t dst_port;
	uint8_t protocol;
};

enum acc_action_type {
	PACKET_FIELD_UPDATE,
};

struct acc_action {
	enum acc_action_type type;
	union {
		struct {
#define ACTION_FIELD_srcmac 		(0X1U <<1)
#define ACTION_FIELD_dstmac        (0X1U <<2)
#define ACTION_FIELD_src_ip         (0X1U <<3)
#define ACTION_FIELD_dst_ip         (0X1U <<4)
#define ACTION_FIELD_src_port       (0X1U <<5)
#define ACTION_FIELD_dst_port       (0X1U <<6)
#define ACTION_FIELD_protocol		(0X1U <<7)
#define ACTION_FIELD_dl_type		(0X1U <<8)
			uint32_t field_mask;
			uint8_t srcmac[6];
			uint8_t dstmac[6];
			uint32_t src_ip;
			uint32_t dst_ip;
			uint16_t src_port;
			uint16_t dst_port;
			uint8_t protocol;
			uint16_t dl_type;
		} field_update;
	} u;
};

struct acc_flow {
	struct acc_match match;
	struct acc_match mask;
	struct acc_action action;
};

typedef enum acc_add_flow_error_code {
	DISCONNECT, //添加流时，发现与加速器间的连接已断开，需要重连
	INVALID, //参数有误
	FLOWS_ACCEPT, //流被加速器接受
	FLOWS_REJECTION, //流被加速器拒绝

} error_code;

#define ACC_ACTION_FIELD_MASK_SET(action,NAME)\
	action.u.field_update.field_mask |= ACTION_FIELD_##NAME

#define ACC_ACTION_FIELD_UPDATE_SET(action,NAME,src)\
do{\
	action.u.field_update.NAME = (src);\
	ACC_ACTION_FIELD_MASK_SET(action,NAME);\
}while(0)

#define ACC_ACTION_IS_FIELD_MASK_SET(action,NAME) \
({action.u.field_update.field_mask & ACTION_FIELD_##NAME;})

static inline int acc_match_is_be_set(uint8_t*mask, int32_t len) {
	int i;
	for (i = 0; i < len; ++i) {
		if (mask[i]) {
			return 1;
		}
	}

	return 0;
}

#define ACC_MATCH_IS_FIELD_MASK_SET(mask,NAME)\
	acc_match_is_be_set((uint8_t*)&(mask.NAME),sizeof(mask.NAME))

#endif /* SRC_FLOW_ACC_FLOW_H_ */

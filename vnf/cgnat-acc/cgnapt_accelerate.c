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

#if 0
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
#endif

void cgnat_accelerate_make_flow(struct acc_flow*cgnat_flow, uint8_t srcmac[6],uint8_t dstmac[6],uint32_t src_ip,
		uint16_t src_port, uint8_t protocol, uint32_t xlate_src_ip,
		uint16_t xlate_src_port, uint8_t xlate_srcmac[6],
		uint8_t xlate_dstmac[6]) {
	assert(cgnat_flow);
	memset(cgnat_flow, 0, sizeof(*cgnat_flow));

	//设置nat的match及mask字段
	acc_match_field_set(&(cgnat_flow->match),&(cgnat_flow->mask),srcmac,srcmac);
	//acc_mac_copy(cgnat_flow->match.srcmac,srcmac);
	//memset(cgnat_flow->mask.srcmac,0xFF,sizeof(cgnat_flow->mask.srcmac));

	acc_match_field_set(&(cgnat_flow->match),&(cgnat_flow->mask),dstmac,dstmac);
	//acc_mac_copy(cgnat_flow->match.dstmac,dstmac);
	//memset(cgnat_flow->mask.dstmac,0xFF,sizeof(cgnat_flow->mask.dstmac));

	acc_match_field_set(&(cgnat_flow->match),&(cgnat_flow->mask),src_ip,src_ip);
	//cgnat_flow->match.src_ip = src_ip;
	//cgnat_flow->mask.src_ip = 0XFFFFFFFF;

	//cgnat_flow->match.dst_ip
	//cgnat_flow->mask.dst_ip

	acc_match_field_set(&(cgnat_flow->match),&(cgnat_flow->mask),src_port,src_port);
	//cgnat_flow->match.src_port = src_port;
	//cgnat_flow->mask.src_port = 0XFFFF;

	//cgnat_flow->match.dst_port;
	//cgnat_flow->mask.dst_port;

	acc_match_field_set(&(cgnat_flow->match),&(cgnat_flow->mask),protocol,protocol);
	//cgnat_flow->match.protocol = protocol;
	//cgnat_flow->mask.protocol = 0XFF;

	//set action:
	acc_action_field_set(&(cgnat_flow->action),srcmac,xlate_srcmac);
	acc_action_field_set(&(cgnat_flow->action),dstmac,xlate_dstmac);
	acc_action_field_set(&(cgnat_flow->action),src_ip,xlate_src_ip);
	acc_action_field_set(&(cgnat_flow->action),src_port,xlate_src_port);
}

#define HELP_SCRIPT()\
{\
	if(writen < 0)\
	{\
		return -1;\
	}\
	use +=writen;\
}
int acc_flows_dumps(char* buffer,int len,struct acc_flow*cgnat_flow)
{
	int use = 0;
	int writen = 0;
	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,srcmac))
	{
		writen = snprintf(buffer+use,len+use," src_mac %02x:%02x:%02x:%02x:%02x:%02x",
				cgnat_flow->match.srcmac[0],cgnat_flow->match.srcmac[1],cgnat_flow->match.srcmac[2],
				cgnat_flow->match.srcmac[3],cgnat_flow->match.srcmac[4],cgnat_flow->match.srcmac[5]);
		HELP_SCRIPT();
	}

	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,dstmac))
	{
		writen = snprintf(buffer+use,len+use," dst_mac %02x:%02x:%02x:%02x:%02x:%02x",
						cgnat_flow->match.dstmac[0],cgnat_flow->match.dstmac[1],cgnat_flow->match.dstmac[2],
						cgnat_flow->match.dstmac[3],cgnat_flow->match.dstmac[4],cgnat_flow->match.dstmac[5]);
		HELP_SCRIPT();
	}

	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,src_ip))
	{
		writen = snprintf(buffer+use,len+use," src_ip  %d.%d.%d.%d",((uint8_t*)&(cgnat_flow->match.src_ip))[0],
				((uint8_t*)&(cgnat_flow->match.src_ip))[1],
				((uint8_t*)&(cgnat_flow->match.src_ip))[2],
				((uint8_t*)&(cgnat_flow->match.src_ip))[3]);
		HELP_SCRIPT();
	}

	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,dst_ip))
	{
		writen = snprintf(buffer+use,len+use," dst_ip  %d.%d.%d.%d",((uint8_t*)&(cgnat_flow->match.dst_ip))[0],
				((uint8_t*)&(cgnat_flow->match.dst_ip))[1],
				((uint8_t*)&(cgnat_flow->match.dst_ip))[2],
				((uint8_t*)&(cgnat_flow->match.dst_ip))[3]);
		HELP_SCRIPT();
	}

	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,src_port))
	{
		writen = snprintf(buffer+use,len+use," src_port  %d",cgnat_flow->match.src_port);
		HELP_SCRIPT();
	}

	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,dst_port))
	{
		writen = snprintf(buffer+use,len+use," dst_port  %d",cgnat_flow->match.dst_port);
		HELP_SCRIPT();
	}

	if(ACC_MATCH_IS_FIELD_MASK_SET(cgnat_flow->mask,protocol))
	{
		writen = snprintf(buffer+use,len+use," protocol  %d",cgnat_flow->match.protocol);
		HELP_SCRIPT();
	}

	writen = snprintf(buffer+use,len+use,"---->");
	HELP_SCRIPT();

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,srcmac))
	{
		writen = snprintf(buffer+use,len+use," xlate_src_mac %02x:%02x:%02x:%02x:%02x:%02x",
						cgnat_flow->action.u.field_update.srcmac[0],cgnat_flow->action.u.field_update.srcmac[1],cgnat_flow->action.u.field_update.srcmac[2],
						cgnat_flow->action.u.field_update.srcmac[3],cgnat_flow->action.u.field_update.srcmac[4],cgnat_flow->action.u.field_update.srcmac[5]);
		HELP_SCRIPT();
	}

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,dstmac))
	{
		writen = snprintf(buffer+use,len+use," xlate_dst_mac %02x:%02x:%02x:%02x:%02x:%02x",
							cgnat_flow->action.u.field_update.dstmac[0],cgnat_flow->action.u.field_update.dstmac[1],cgnat_flow->action.u.field_update.dstmac[2],
							cgnat_flow->action.u.field_update.dstmac[3],cgnat_flow->action.u.field_update.dstmac[4],cgnat_flow->action.u.field_update.dstmac[5]);
		HELP_SCRIPT();
	}

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,src_ip))
	{
		writen = snprintf(buffer+use,len+use," xlate_src_ip  %d.%d.%d.%d",((uint8_t*)&(cgnat_flow->action.u.field_update.src_ip))[0],
				((uint8_t*)&(cgnat_flow->action.u.field_update.src_ip))[1],
				((uint8_t*)&(cgnat_flow->action.u.field_update.src_ip))[2],
				((uint8_t*)&(cgnat_flow->action.u.field_update.src_ip))[3]);
		HELP_SCRIPT();
	}

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,dst_ip))
	{
		writen = snprintf(buffer+use,len+use," xlate_dst_ip  %d.%d.%d.%d",((uint8_t*)&(cgnat_flow->action.u.field_update.dst_ip))[0],
				((uint8_t*)&(cgnat_flow->action.u.field_update.dst_ip))[1],
				((uint8_t*)&(cgnat_flow->action.u.field_update.dst_ip))[2],
				((uint8_t*)&(cgnat_flow->action.u.field_update.dst_ip))[3]);
		HELP_SCRIPT();
	}

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,src_port))
	{
		writen = snprintf(buffer+use,len+use," xlate_src_port  %d",cgnat_flow->action.u.field_update.src_port);
		HELP_SCRIPT();
	}

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,dst_port))
	{
		writen = snprintf(buffer+use,len+use," xlate_dst_port  %d",cgnat_flow->action.u.field_update.dst_port);
		HELP_SCRIPT();
	}

	if(ACC_ACTION_IS_FIELD_MASK_SET(cgnat_flow->action,protocol))
	{
		writen = snprintf(buffer+use,len+use," xlate_protocol  %d",cgnat_flow->action.u.field_update.protocol);
		HELP_SCRIPT();
	}

	writen = snprintf(buffer+use,len+use,"\n");
	HELP_SCRIPT();
	return 0;
}

#undef HELP_SCRIPT

int cgnat_accelerate(void*id,struct acc_flow* cgnat_flow)
{

	error_code ret = DISCONNECT;
	assert(id);
	assert(cgnat_flow);

	struct acc_client*client = acc_client_register(id);
	assert(client);
	
#if 1
	char buffer[4096]={0};
	if(!acc_flows_dumps(buffer,4096,cgnat_flow))
	{
		printf("%s",buffer);
	}
#endif
#if 0
	cgnat_accelerate_make_flow(&cgnat_flow,srcmac,dstmac, src_ip, src_port, protocol,
			xlate_src_ip, xlate_src_port, xlate_srcmac, xlate_dstmac);
#endif
	if (acc_client_is_connected(client)) {
		ret = acc_client_add_flows(client, cgnat_flow, 1);
	}

	if (ret == DISCONNECT) {
		if (!acc_client_connect(client)) {
			return DISCONNECT;
		}
		ret = acc_client_add_flows(client, cgnat_flow, 1);
	}

	return ret == FLOWS_ACCEPT;
}

#if FOR_TEST
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
int cgnat_accelerate_test(void*id,uint8_t srcmac[6],uint8_t dstmac[6], uint32_t src_ip, uint16_t src_port,
		uint8_t protocol, uint32_t xlate_src_ip, uint16_t xlate_src_port,
		uint8_t xlate_srcmac[6], uint8_t xlate_dstmac[6])
{
	struct acc_flow cgnat_flow;
	cgnat_accelerate_make_flow(&cgnat_flow,srcmac,dstmac, src_ip, src_port, protocol,
				xlate_src_ip, xlate_src_port, xlate_srcmac, xlate_dstmac);
	return cgnat_accelerate(id,&cgnat_flow);
}

int main(int argc,char**argv)
{
	uint8_t mac1[6]="abcdef",mac2[6]="12345";
	printf("%d\n",cgnat_accelerate_test((void*)1,mac1,mac2,3,4,6,6,7,mac1,mac2));
	printf("%d\n",cgnat_accelerate_test((void*)1,mac1,mac2,3,4,6,6,7,mac1,mac2));
	return 0;
}
#endif

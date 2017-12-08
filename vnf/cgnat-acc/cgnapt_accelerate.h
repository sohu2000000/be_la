#ifndef _CGNAPT_ACCELERATE_H_

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
int cgnat_accelerate(void*id, uint8_t srcmac[6],uint8_t dstmac[6],uint32_t src_ip,
		uint16_t src_port, uint8_t protocol, uint32_t xlate_src_ip,
		uint16_t xlate_src_port, uint8_t xlate_srcmac[6],
		uint8_t xlate_dstmac[6]);
#endif

#ifndef __BE_LA_CMD_H__
#define __BE_LA_CMD_H__

#include "be_la_vendor.h"


enum tag_cmd_type {
	CMD_TYPE_REGISTER = 1,
    CMD_TYPE_UNREGISTER,
    CMD_TYPE_MAX
};

#define ACC_CARD_MAX_NUM      64
#define VMPATH_MAX_LEN        128
#define VMUUID_MAX_LEN        128

typedef struct {
	enum tag_vendor vendor;
	enum tag_model model;
    enum tag_acc_type acc_type;
    char vm_uuid[VMUUID_MAX_LEN];
    char vmpath[VMPATH_MAX_LEN];  /*Unique index*/
}acc_card_t;

typedef struct tag_cmd_hdr{
	acc_card_t card;
    enum tag_cmd_type cmd_type;
} cmd_hdr_t;


int be_la_cmd_process(cmd_hdr_t * cmd, void*arg);

#endif

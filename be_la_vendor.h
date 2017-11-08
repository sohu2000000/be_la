#ifndef __BE_LA_VENDOR_H__
#define __BE_LA_VENDOR_H__

#include <memory.h>
#include "be_la_cmd.h"

enum tag_vendor {
	VENDOR_NETRONOME = 1, 
    VENDOR_XILINX,
    VENDOR_INTEL,
    VENDOR_CAVIUM,
    VENDOR_MALLNOX,
    VENDOR_MAX
};

enum tag_model {
    /*NETRONOME*/
    AGILIO_ISA_4000 = 1, 
    XILINX_VU3P,
    XILINX_VU9P,
    MLNX_CX516A,
    MODEL_MAX
};

enum tag_acc_type {
	ACC_TYPE_FLOW = 1, 
    ACC_TYPE_CRYPTO,
    ACC_TYPE_DECRYPTO,
    ACC_TYPE_COMPRESS,
    ACC_TYPE_UNCOMPRESS,
    ACC_TYPE_MAX
};

#define PATH_LEN        128
#define UUID_LEN        128
#define ACC_CARD_NUM    64

typedef struct tag_acc_card{
    unsigned int vendor;
    unsigned int model;
    unsigned int acc_type;
    int channel_fd;
    char vm_uuid[UUID_LEN];
    char vmpath[PATH_LEN];
}acc_card_t;

typedef struct tag_acc_cards{
    acc_card_t  * acc_cards[ACC_CARD_NUM];    
    int reg_num;
}acc_cards_t;


#define REG_ACC_CARD_VENDOR(entry, vendor_info)  \
    {\
        (entry)->vendor = (vendor_info); \
    }

#define REG_ACC_CARD_MODEL(entry, model_info)  \
    {\
        (entry)->model = (model_info); \
    }


#define REG_ACC_CARD_ACCTYPE(entry, acc_type_info)  \
    {\
        (entry)->acc_type = (acc_type_info); \
    }

#define REG_ACC_CARD_VMUUID(entry, uuid_info)  \
    {\
        memcpy((entry)->vm_uuid, (uuid_info), PATH_LEN); \
    }

#define REG_ACC_CARD_VMPATH(entry, path_info)  \
    {\
        memcpy((entry)->vmpath, (path_info), PATH_LEN); \
    }


int be_la_acc_card_reg(cmd_hdr_t * reginfo);
void be_la_acc_card_dump(void);
int be_la_vendor_init(void);
void be_la_vendor_exit(void);

#endif

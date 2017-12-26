#ifndef __BE_LA_VENDOR_H__
#define __BE_LA_VENDOR_H__

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
    /*INTEL QAT*/
    QAT_DH8950,
    MODEL_MAX
};

//suport 'OR'
enum tag_acc_type {
	ACC_TYPE_FLOW =       0x1,
    ACC_TYPE_CRYPTO = 	  0x2,
    ACC_TYPE_DECRYPTO =   0x4,
    ACC_TYPE_COMPRESS =   0x8,
    ACC_TYPE_UNCOMPRESS = 0x16,
    ACC_TYPE_MAX
};


int be_la_vendor_init(void);
void be_la_vendor_exit(void);

#endif

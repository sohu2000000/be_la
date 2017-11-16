#ifndef __BE_LA_CMD_H__
#define __BE_LA_CMD_H__

#define PATH_LEN    128
#define UUID_LEN    128

typedef struct tag_cmd_hdr{
    int cmd_type;
    int vendor;
    int model;
    int acc_type;   //bit map
    char vm_uuid[UUID_LEN];    
    char vmpath[PATH_LEN];
} cmd_hdr_t;


enum tag_cmd_type {
	CMD_TYPE_REGISTER = 1, 
    CMD_TYPE_UNREGISTER, 
    CMD_TYPE_MAX
};

int be_la_cmdsrv_init(char * socket_path);
int be_la_cmd_process(void * data);

#endif
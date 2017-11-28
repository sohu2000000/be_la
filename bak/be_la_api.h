#ifndef __BE_LA_API_H__
#define __BE_LA_API_H__


#define BUF_LEN 2048

enum tag_msg_type{
    MSG_TYPE_DATA = 1,
    MSG_TYPE_DATA_MORE,      
    MSG_TYPE_MAX
};

typedef struct tag_acc_proc_msg{
    unsigned int msg_type;
    unsigned int acc_type;
    char buffer[BUF_LEN];
}acc_proc_msg_t;


#endif

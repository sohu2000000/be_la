#ifndef __BE_LA_ACCPROC_H__
#define __BE_LA_ACCPROC_H__


#ifdef MSG_DEBUG_ON
#define MSG_DEBUG_OUTPUT  printf

#ifdef MSG_ATTR_USED
    #undef MSG_ATTR_USED
#endif
#define MSG_ATTR_USED

#else
#define MSG_DEBUG_OUTPUT(args...)  
#ifdef MSG_ATTR_USED
    #undef MSG_ATTR_USED
#endif
#define MSG_ATTR_USED   __attribute__ ((unused))
#endif


void msg_debug_buffer_dump(char * buf MSG_ATTR_USED, unsigned int len MSG_ATTR_USED);
int be_la_acc_process(int vendor, int model, void * inbuf, unsigned int in_len, void * outbuf, int out_len);

#endif

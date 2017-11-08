#ifndef __BE_LA_THD_H__
#define __BE_LA_THD_H__

#include <pthread.h>

#define CMD_MSG_LEN 1024

typedef void* (* pTheadFunc)(void * arg);

typedef struct tag_thd_msg{
    pthread_mutex_t cmd_lock;
    void * cmd_msg;
}thd_msg_t;

void * vm_channel_thread(void * data);
int thread_init(pTheadFunc pfunc, void * data);
int thread_module_init(void);
void thread_module_exit(void);


#endif

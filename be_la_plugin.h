#ifndef __BE_LA_PLUGIN_H__
#define __BE_LA_PLUGIN_H__

#include "be_la_vendor.h"

typedef void* (* pPluginFunc)(void * data);


typedef struct tag_acc_plugin{
    pPluginFunc pfunc;    
}acc_plugin_t;


int be_la_plugin_reg(int vendor, int model, pPluginFunc pfunc);
int be_la_plugin_init(void);
void be_la_plugin_exit(void);



#endif


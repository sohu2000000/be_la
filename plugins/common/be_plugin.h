#ifndef __BE_LA_PLUGIN_H__
#define __BE_LA_PLUGIN_H__

#include <stdint.h>
#include "be_la_vendor.h"

typedef int (* pPluginFunc)(void * inbuf, uint32_t in_len, void * outbuf, uint32_t * out_len_ptr);


typedef struct acc_plugin{
    pPluginFunc pfunc;
    char name[32];
    enum tag_vendor vendor;
    enum tag_model  model;
}acc_plugin_t;


int be_plugin_register(acc_plugin_t* plugin);
int be_plugin_unregister(acc_plugin_t* plugin);
int be_plugin_is_registed(enum tag_vendor vendor,enum tag_model model);
int be_plugin_callback(enum tag_vendor vendor,enum tag_model model,void*inbuf,uint32_t in_len,void*outbuf,uint32_t*out_len);

#endif


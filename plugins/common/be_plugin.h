#ifndef __BE_LA_PLUGIN_H__
#define __BE_LA_PLUGIN_H__

#include <stdint.h>

#include "be_la_vendor.h"
#include "be_acc_context.h"
#include "be_la_cmd.h"

struct be_acc_card_;

typedef int (* pPluginFunc)(struct be_acc_card_* context,void * inbuf, uint32_t in_len, void * outbuf, int32_t * out_len_ptr);
typedef int (* pPluginInit)(void);
typedef int (* pPluginDestroy)(void);
typedef void* (* pPluginContextInit)(struct be_acc_card_*);
typedef int (* pPluginContextDestroy)(struct be_acc_card_*);

typedef struct acc_plugin{
    pPluginFunc pfunc;
    pPluginInit init;
    pPluginDestroy destroy;
    pPluginContextInit context_init;
    pPluginContextDestroy context_destroy;
    char name[32];
    enum tag_vendor vendor;
    enum tag_model  model;
    enum tag_acc_type acc_type;
}acc_plugin_t;


int be_plugin_register(acc_plugin_t* plugin);
int be_plugin_unregister(acc_plugin_t* plugin);
int be_plugin_is_registed(enum tag_vendor vendor,enum tag_model model);
//int be_plugin_callback(be_card_inner_t*card,void*inbuf,uint32_t in_len,void*outbuf,int32_t*out_len);
acc_plugin_t* be_plugin_match(acc_card_t*card);

void* be_plugin_context_init(struct be_acc_card_*card,acc_plugin_t*plugin);
int be_plugin_context_destory(struct be_acc_card_*card,acc_plugin_t*plugin);
int be_plugin_init(acc_plugin_t* plugin);
int be_plugin_destroy(acc_plugin_t* plugin);
int be_plugin_process(struct be_acc_card_*card,acc_plugin_t* plugin,void*inbuf,uint32_t in_len,void*outbuf,int32_t*out_len);
#endif


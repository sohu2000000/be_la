/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_plugin.c
*
* DESCRIPTION:
* 
* AUTHOR:
*   Feng Liu 626987
*
* CREATED DATE:
*   28/10/2017
* REVISION:
*   1.0
*
* MODIFICATION HISTORY
* --------------------
* $Log:$
*   Feng Liu 28/10/2017  Creation
*****************************************************************/

#include <memory.h>

#include "be_la_log.h"
#include "be_la_plugin.h"
#include "be_la_api.h"

acc_plugin_t g_acc_plugins[VENDOR_MAX][MODEL_MAX];

/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
void * agilio_isa_4000_proccess(void * data){
    
    acc_proc_msg_t * msg = NULL;

    if(NULL == data){
        BE_LA_LOG("agilio_isa_4000_proccess\n");
        return NULL;
    }

    msg = (acc_proc_msg_t*)data;
    
    BE_LA_LOG("agilio_isa_4000_proccess acc_type: 0x%x, buffer: %s\n", 
        msg->acc_type, msg->buffer);
    
    return NULL;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
/*plugin reg interface*/
int be_la_plugin_reg(int vendor, int model, pPluginFunc pfunc){
    int retval = 0;

    if(g_acc_plugins[vendor][model].pfunc){
        BE_LA_ERROR("vendor: %d, model: %d has already registed!\n", vendor, model);
        retval = -1;
    }

    g_acc_plugins[vendor][model].pfunc = pfunc;

out:
    return retval;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
int be_la_plugin_unreg(int vendor, int model){
    g_acc_plugins[vendor][model].pfunc = NULL;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
int be_la_plugin_init(void){
    int retval = 0;
    
    memset(g_acc_plugins, 0x0, sizeof(g_acc_plugins));
    be_la_plugin_reg(VENDOR_NETRONOME, AGILIO_ISA_4000, agilio_isa_4000_proccess);
    
    return retval;
}


/******************************************************************************
* DESCRIPTION:
*       none
* INPUTS:
*       none
* OUTPUTS:
*       none
* RETURNS:
*       none
******************************************************************************/
void be_la_plugin_exit(void){

    memset(g_acc_plugins, 0x0, sizeof(g_acc_plugins));
    return;
}




/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_cmd.c
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


#include <stdlib.h>
#include <string.h>

#include "be_la_comm.h"
#include "be_la_log.h"
#include "be_la_cmd.h"
#include "be_la_thd.h"
#include "be_la_vendor.h"

extern thd_msg_t g_thd_msg;

#include "be_la_plugin.h"
extern acc_plugin_t g_acc_plugins[VENDOR_MAX][MODEL_MAX];


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
int be_la_cmd_process(void * data)
{
    int retval = 0;
    cmd_hdr_t * cmd = NULL;
    
    if(NULL == data){
        BE_LA_ERROR("cmd message is NULL");
    }

    pthread_mutex_lock(&g_thd_msg.cmd_lock);
    
    memcpy(g_thd_msg.cmd_msg, data, CMD_MSG_LEN);       
    cmd = (cmd_hdr_t * )g_thd_msg.cmd_msg;
    //BE_LA_LOG("recv message from client: cmd = %d, vmpath = %s\n", cmd->cmd_type, cmd->vmpath);
    //BE_LA_LOG("recv message from client: vendor = %d, acc_type = %d\n", cmd->vendor, cmd->acc_type);

    switch (cmd->cmd_type) {
    case CMD_TYPE_REGISTER:
        retval = be_la_acc_card_reg(cmd); 
        if(-1 == retval){
            BE_LA_ERROR("be_la_acc_card_reg failed: %d\n", retval);
            goto out;
        }        

        /*temp code*/
        be_la_acc_card_dump();
        
        /*init a thread to process a vm channel*/        
        retval = thread_init(vm_channel_thread, (void *)cmd);
        if(-1 == retval){
            BE_LA_ERROR("thread_init failed: %d\n", retval);
            goto out;
        }    
        break;
    case CMD_TYPE_UNREGISTER:
        // TODO: 
        break;
    default:
        BE_LA_ERROR("unsupported command type\n");
        retval = -1;
        goto out;
    
    }

out:
    pthread_mutex_unlock(&g_thd_msg.cmd_lock);
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
int be_la_cmdsrv_init(char * socket_path){
    int retval = 0;

    if(NULL == socket_path){
        BE_LA_ERROR("socket_path is NULL\n");
        retval = -1;
        goto out;
    }

    retval = unix_socket_srv_init(socket_path);

out:
    return retval;
}




/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_accproc.c
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

#include <sys/types.h>
#include <stdio.h>

#include "be_la_accproc.h"
#include "be_la_plugin.h"
#include "be_la_log.h"
#include "be_la_api.h"
#include "be_la_common.h"

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
void msg_debug_buffer_dump(char * buf MSG_ATTR_USED, unsigned int len MSG_ATTR_USED)
{
#ifdef MSG_DEBUG_ON

    uint32_t i,j,k=0;
    uint32_t row = 0;
    uint32_t colum = 4;
    char_t *cursor = buf;
    char_t padding[32] = "      "; 
    if ((NULL == buf)
        || (0 == len))
    {
        return;
    }

    MSG_DEBUG_OUTPUT("[MSG] buffer dump(pos:0x%08x lenth:%u):\r\n", buf, len);

    row = (len % (4*colum) == 0) ? (len / (4*colum)) : (len / (4*colum) + 1);

    for (i = 0; i < row; i++)
    {
        MSG_DEBUG_OUTPUT("  0x%08x:%s", cursor, padding);
        for (j = 0; j < colum; j ++)
        {
            MSG_DEBUG_OUTPUT("0x%08x", *(uint32_t *)cursor);
            cursor += 4;
            k += 4;
            MSG_DEBUG_OUTPUT("%s", padding);
            if (k >= len)
            {
                MSG_DEBUG_OUTPUT("\r\n\r\n");
                return;
            }
        }
        MSG_DEBUG_OUTPUT("\r\n");
    }
    MSG_DEBUG_OUTPUT("\r\n\r\n");
#else
    /* suppress param not use info */
#endif

    return;
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
int be_la_acc_process(int vendor, int model,
                            void * inbuf, unsigned int in_len, 
                            void * outbuf, int * out_len_ptr){
    int retval = 0;    
    acc_proc_msg_t * msg = NULL;
    
    if(NULL == inbuf){
        BE_LA_ERROR("data is NULL");
        retval = -1;
    }

    msg = (acc_proc_msg_t *)inbuf;    
    msg_debug_buffer_dump(inbuf, in_len);
    switch(msg->msg_type){
    case MSG_TYPE_DATA:
    case MSG_TYPE_DATA_MORE:
        if(NULL == g_acc_plugins[vendor][model].pfunc){
            BE_LA_ERROR("g_acc_plugins[%d][%d].pfunc is not registed \n", vendor, model);
            break;
        }
        g_acc_plugins[vendor][model].pfunc(inbuf); 
        break;
    
    case MSG_TYPE_MAX:
    default:
        BE_LA_LOG("unsupport msg type: %d\n", msg->msg_type);
        break;        
    }
        
out:
    return retval;

}




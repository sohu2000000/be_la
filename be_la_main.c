/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_main.c
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
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "be_la_main.h"
#include "be_la_thd.h"
#include "be_la_cmd.h"
#include "be_la_log.h"
#include "be_la_vendor.h"
#include "be_la_plugin.h"


#define VSERIAL_UNIX_PATH   "/tmp/vserial"
#define CMD_UNIX_PATH       "/tmp/be_la_cmd"


BE_LA_COMM_HANDLE g_handler;


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
int main(void) {

    int retval = 0;

#if 0
    /* become a deamon, no change dir and not redirect io */
    if(daemon(1,1) < 0){
        perror("deamon failed: ");
        retval = -1;
        goto out;
    }
#endif

    retval = be_la_vendor_init();
    retval = be_la_plugin_init();
    retval = thread_module_init();
    
    /*last to initialization, since it is a for(;;)*/
    retval = be_la_cmdsrv_init(CMD_UNIX_PATH);    
        
out:
    thread_module_exit();        
    be_la_plugin_exit();
    be_la_vendor_exit();
    return retval;

}




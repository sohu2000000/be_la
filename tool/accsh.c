/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   accsh.c
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

#include "accsh_comm.h"
#include "accsh.h"


#define CMD_UNIX_PATH       "/tmp/be_la_cmd"
#define VSERIAL_UNIX_PATH   "/tmp/vserial"


ACCSH_HANDLE g_handler;


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
int main(int argc, char** argv) {

    int read_size = -1, num = -1, maxfd  = -1, retval = 0, total = 0;
    char buffer[1024] = {"This is a commond\n"}, *path = NULL;
    char * data = NULL;


    total = 1024;
    data = (void *)malloc(total);
    if(NULL == data){
        perror("malloc failed: ");
        retval = -1;
        goto out;
    }
        

    /*init a unix socket cline with tcp link*/
    g_handler.cmdsrv_fd = unix_socket_clnt_init(CMD_UNIX_PATH);
    if(-1  == g_handler.cmdsrv_fd){
        perror("init_unix_socket failed: ");
        goto out;
    }


    cmd_hdr_t * cmd = (cmd_hdr_t * )data;
    cmd->cmd_type = CMD_TYPE_REGISTER;
    cmd->vendor = VENDOR_NETRONOME;
    cmd->model = AGILIO_ISA_4000;
    cmd->acc_type = 1 << ACC_TYPE_FLOW | 1 << ACC_TYPE_COMPRESS;
    strcpy(cmd->vmpath, VSERIAL_UNIX_PATH);
    strcpy(cmd->vm_uuid, "4dea22b3-1d52-d8f3-2516-782e98000000");
    
    write(g_handler.cmdsrv_fd, data, total);


out1:
    free(data);
out:
    return retval;

}




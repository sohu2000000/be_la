/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_log.c
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
#include "be_la_log.h"
#include "be_la_comm.h"
#include "be_la_thd.h"
#include "be_la_api.h"
#include "be_la_cmd.h"

extern BE_LA_COMM_HANDLE g_handler;

thd_msg_t g_thd_msg;


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
void * vm_channel_thread(void * data){

    int read_size = -1, num = -1, maxfd  = -1, in_len = 0, out_len = 0;
    int channel_vendor = 0, channel_model = 0; 
    char buffer[BUF_LEN] = {'\0'}, feedback[BUF_LEN] = {'\0'}, path[BUF_LEN] = {'\0'};
    acc_proc_msg_t inbuf, outbuf;
    cmd_hdr_t * cmd = NULL;
    fd_set rfds;

    /*init a unix socket cline with tcp link*/
    if(NULL == data){
        BE_LA_LOG("data is NULL\n");
    }

    /*record vm channel spec info */
    cmd = (cmd_hdr_t *)data;
    channel_vendor = cmd->vendor;
    channel_model = cmd->model;
    strcpy(path, cmd->vmpath);    
    BE_LA_LOG("path = %s\n", path);

    
    g_handler.vseria_fd = unix_socket_clnt_init(path);
    if(-1  == g_handler.vseria_fd){
        perror("init_unix_socket failed: ");
        goto out;
    }


    /* listen the unix socket link */
    maxfd = g_handler.vseria_fd + 1;    
    for(;;){
        num = 0;        
        // TODO: 
        FD_ZERO(&rfds);                
        FD_SET(g_handler.vseria_fd, &rfds);

        num = select(maxfd, &rfds, NULL, NULL, NULL);  
        if(num < 0)
        {
            perror("select error\n");
            goto out;
        }
        else if (num > 0)
        {
            if(FD_ISSET(g_handler.vseria_fd, &rfds))
            {
                /* 1. read meessage from VM */
                in_len = 0;
                out_len = 0;
                memset(&inbuf, 0x0, sizeof(inbuf));
                memset(&outbuf, 0x0, sizeof(outbuf));
                
                in_len = read(g_handler.vseria_fd, &inbuf, sizeof(inbuf));
                if(in_len > 0){
                    BE_LA_LOG("read_size = %d\n", in_len);                    
                } 

                // 2. TODO: process messsage
                
                be_la_acc_process(channel_vendor, channel_model, &inbuf, in_len, &outbuf, out_len);
                //memset(buffer, 0x0, sizeof(buffer));

                /*give the feedback*/
                strcpy(feedback, "Got It, I am Hypervisor");   
                
                /* 3. write result to VM */
                BE_LA_LOG("Please write the feedback message: \n");                   
                if (-1 == write(g_handler.vseria_fd, feedback, sizeof(feedback))){
                    perror("write feedback failed: ");
                    break;
                }

            }
        }        
    }


out:
    BE_LA_LOG("close fd = %d\n", g_handler.vseria_fd);
    close(g_handler.vseria_fd);
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
int thread_init(pTheadFunc pfunc, void * data){
    pthread_t t; 
    int retval = 0;
    
    if (pthread_create(&t, NULL, pfunc, data) != 0)
    {
        perror("pthread_create failed");
        retval = -1;
        goto out;
    }
    
    pthread_detach(t);
    
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
int thread_module_init(void){
    int retval = 0;

    pthread_mutex_init(&g_thd_msg.cmd_lock, NULL);
    g_thd_msg.cmd_msg = (void *)malloc(CMD_MSG_LEN * sizeof(char));
    if(NULL == g_thd_msg.cmd_msg){
        BE_LA_ERROR("g_thd_msg.cmd_msg alloc failed");
        retval = -1;
        goto out;
    }

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
void thread_module_exit(void){

    if(g_thd_msg.cmd_msg){
        free(g_thd_msg.cmd_msg);
        g_thd_msg.cmd_msg = NULL;
    }

    pthread_mutex_destroy(&g_thd_msg.cmd_lock);

    return;

}




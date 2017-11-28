/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_comm.c
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


#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include "bak/be_la_main.h"
#include "be_la_cmd.h"
#include "common/be_la_log.h"


/******************************************************************************
* DESCRIPTION:
*       Init a unix socket 
* INPUTS:
*       @socket_path unix socket path
* OUTPUTS:
*       none
* RETURNS:
*       unix socket file desc
******************************************************************************/
int unix_socket_clnt_init(char * socket_path){
    int sockfd  = 0, ret = 0;
    struct sockaddr_un address;

    /*UNIX SOCKET*/
    if(access(socket_path, W_OK) < 0)
    {
        perror("access failed: ");
        return -1;
    }   
    
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        perror("socket failed: ");
        return -1;
    }   
    
    bzero((void *)&address, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy((char*)address.sun_path, socket_path, sizeof(struct sockaddr_un) - 1);
    
    ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(ret == -1){
        perror("connect failed: ");
        return -1;
    }


    BE_LA_LOG("init_unix_socket sockfd = %d\n", sockfd);
    return sockfd;
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
int unix_socket_srv_init(char * socket_path){
#define BUF_LEN 1024
    int server_fd = 0, client_fd = 0, retval = 0, client_len = 0;
    struct sockaddr_un server_addr, client_addr;
    //char buffer[105] = {'\0'}, send[105] = {'\0'};
    char buffer[BUF_LEN] = {'\0'},  send[BUF_LEN] = {'\0'}, *msg = NULL;

    if(0 <= access(socket_path, W_OK)) {
        remove(socket_path);
    }
    
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(-1 == server_fd){
        perror("socket failed: ");
        retval = -1;
        goto out;
    }
    
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socket_path);
    if(-1 == bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))){
        perror("bind: ");
        retval = -1;
        goto out;
    }

    listen(server_fd, 10);  //server listen most 10 requests.
    BE_LA_LOG("server is listening: \n");
    client_len = sizeof(client_addr);
   
    for(;;){
        memset(buffer, 0x0, sizeof(BUF_LEN * sizeof(char)));
       
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (int *)&client_len);
        if(client_fd == -1){
            perror("accept: ");
            retval = -1;
            goto out1;
        }        

        if(-1 == read(client_fd, buffer, BUF_LEN)){
            perror("read failed: ");
        }

        // process cmd line
        retval = be_la_cmd_process((void *) buffer);        
        if( -1 == retval){
            BE_LA_ERROR("be_la_cmd_process failed \n");
            goto out1;
        }
        

        close(client_fd);
    }


out1:    
    close(server_fd);
    remove(socket_path);

out:
    return retval;
    
}



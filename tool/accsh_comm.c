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

#include <sys/un.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>


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
    strncpy((void *)address.sun_path, socket_path, sizeof(struct sockaddr_un) - 1);
    
    ret = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(ret == -1){
        perror("connect failed: ");
        return -1;
    }


    return sockfd;
}



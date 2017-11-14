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
#include <getopt.h>

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
static void usage(void){
    fprintf(stderr, "Usage: \n");
    fprintf(stderr, "\t <-v, --vendor>            acc card vendor: <netronome | xilinx> \n");
    fprintf(stderr, "\t <-m, --model>             acc card product: <agilio_isa_4000 | vu3p | vu9p> \n");
    fprintf(stderr, "\t <-u, --uuid>              vm uuid which to use this channel\n");
    fprintf(stderr, "\t <-c, --channel>           channel path\n");    
    fprintf(stderr, "\t <-t, --type>              acc type: <flow | compress>\n");
    fprintf(stderr, "Example: \n");
    fprintf(stderr, "\taccsh -v netronome -m agilio_isa_4000 -u 4dea22b3-1d52-d8f3-2516-782e98000000 -c /tmp/vserial -t flow -t compress\n");
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
int main(int argc, char** argv) {
    int c;
    int read_size = -1, num = -1, maxfd  = -1, retval = 0, total = 0;
    char buffer[1024] = {"This is a commond\n"}, *path = NULL, *param;
    char * data = NULL;
    cmd_hdr_t * cmd = NULL;

    /*long options*/
    static const char * optstring = "v:m:c:u:t:";
	static const struct option longopts[] = {
		{ "vendor",     required_argument,  NULL, 'v' },
		{ "model",	    required_argument,  NULL, 'm' },
		{ "channel",    required_argument,  NULL, 'c' },
		{ "uuid",       required_argument,  NULL, 'u' },
		{ "type",       required_argument,  NULL, 't' },
		{ "version",    no_argument,        NULL, 'V' },
		{ NULL,		    no_argument,        NULL, 0 }
	};



    total = 1024;
    data = (void *)malloc(total);
    if(NULL == data){
        perror("malloc failed: ");
        retval = -1;
        goto out;
    }
    memset(data, 0x0, total);
        
    cmd = (cmd_hdr_t * )data;
    while(-1 != (c = getopt_long(argc, argv, optstring, longopts, NULL))){
        #if 0
        printf("c = %c\n",c);
        printf("optarg = %s\n", optarg);
        printf("optind = %d\n", optind);
        printf("argv[optind - 1] = %s\n", argv[optind - 1]);
        #endif
        
        switch(c){
        case 'v':
            if(0 == strcmp(optarg,"netronome")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->vendor = VENDOR_NETRONOME;
            }
            break;
        case 'm':
            if(0 == strcmp(optarg,"agilio_isa_4000")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->model = AGILIO_ISA_4000;
            }            
            break;
        case 'c':
            if(optarg[0] != '\0'){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);            
                strcpy(cmd->vmpath, optarg);
            }
            break;
        case 'u':
            if(optarg[0] != '\0'){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);            
                strcpy(cmd->vm_uuid, optarg);
            }
            break;
        case 't':
            if(0 == strcmp(optarg,"flow")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->acc_type |= 1 << ACC_TYPE_FLOW;
                printf("cmd->acc_type = 0x%x\n", cmd->acc_type);
            } else if (0 == strcmp(optarg,"compress")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->acc_type |= 1 << ACC_TYPE_COMPRESS;
            } else{
                fprintf(stderr, "unsupported acc type\n");
            }            
            break;            
        default:
            fprintf(stderr, "unsupport param: %c\n", c);
            break;
        }
        
    }


    if(0 == cmd->vendor 
        || 0 == cmd->model 
        || 0 == cmd->acc_type 
        || '\0' == cmd->vmpath[0] 
        || '\0' == cmd->vm_uuid[0]){
        fprintf(stderr, "Bad Usage\n");
        retval = -1;
        goto out3;
    }    

    /*init a unix socket cline with tcp link*/
    g_handler.cmdsrv_fd = unix_socket_clnt_init(CMD_UNIX_PATH);
    if(-1  == g_handler.cmdsrv_fd){
        perror("init_unix_socket failed: ");
        goto out1;
    }
    
    write(g_handler.cmdsrv_fd, data, total);
    goto out2;

out3:
    usage();
out2:
    free(data);
out1:
    close(g_handler.cmdsrv_fd);
out:
    return retval;

}




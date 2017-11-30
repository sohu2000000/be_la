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
#include "be_la_path.h"

//#define CMD_UNIX_PATH       "/tmp/be_la_cmd"
//#define VSERIAL_UNIX_PATH   "/tmp/vserial"


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
    fprintf(stderr, "\t <-r, --register>          register an acc card \n");    
    fprintf(stderr, "\t <-R, --unregister>        unregister an acc card \n");
    fprintf(stderr, "\t <-v, --vendor>            acc card vendor: <netronome | xilinx> \n");
    fprintf(stderr, "\t <-m, --model>             acc card product: <agilio_isa_4000 | vu3p | vu9p> \n");
    fprintf(stderr, "\t <-u, --uuid>              vm uuid which to use this channel\n");
    fprintf(stderr, "\t <-c, --channel>           channel path\n");    
    fprintf(stderr, "\t <-t, --type>              acc type: <flow | compress>\n");
    fprintf(stderr, "Example: \n");
    fprintf(stderr, "\taccsh -r -v netronome -m agilio_isa_4000 -u 4dea22b3-1d52-d8f3-2516-782e98000000 -c /tmp/vaccla -t flow -t compress\n");
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
    int retval = 0;
    char * data = NULL;
    cmd_hdr_t * cmd = NULL;

    /*long options*/
    static const char * optstring = "v:m:c:u:t:rRV";
	static const struct option longopts[] = {
		{ "vendor",     required_argument,  NULL, 'v' },
		{ "model",	    required_argument,  NULL, 'm' },
		{ "channel",    required_argument,  NULL, 'c' },
		{ "uuid",       required_argument,  NULL, 'u' },
		{ "type",       required_argument,  NULL, 't' },
		{ "register",   no_argument,        NULL, 'r' },
		{ "unregister", no_argument,        NULL, 'R' },
		{ "version",    no_argument,        NULL, 'V' },
		{ NULL,		    no_argument,        NULL, 0 }
	};



    //total = 1024;
    //data = (void *)malloc(total);
	cmd = (cmd_hdr_t * )malloc(sizeof(*cmd));
    if(NULL == cmd){
        perror("malloc failed: ");
        retval = -1;
        goto out;
    }
    memset(cmd, 0x0, sizeof(*cmd));
        
    while(-1 != (c = getopt_long(argc, argv, optstring, longopts, NULL))){        
        switch(c){
        case 'v':
            if(0 == strcmp(optarg,"netronome")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->card.vendor = VENDOR_NETRONOME;
            }
            break;
        case 'm':
            if(0 == strcmp(optarg,"agilio_isa_4000")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->card.model = AGILIO_ISA_4000;
            }            
            break;
        case 'c':
            if(optarg[0] != '\0'){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);            
                strcpy(cmd->card.vmpath, optarg);
            }
            break;
        case 'u':
            if(optarg[0] != '\0'){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);            
                strcpy(cmd->card.vm_uuid, optarg);
            }
            break;
        case 'r':
            printf("c = %c\n",c);
            printf("optarg = %s\n", optarg);                   
            cmd->cmd_type = CMD_TYPE_REGISTER;
            break;
        case 'R':
            printf("c = %c\n",c);
            printf("optarg = %s\n", optarg);                   
            cmd->cmd_type = CMD_TYPE_UNREGISTER;
            break;
        case 't':
            if(0 == strcmp(optarg,"flow")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->card.acc_type |= 1 << ACC_TYPE_FLOW;
                printf("cmd->acc_type = 0x%x\n", cmd->card.acc_type);
            } else if (0 == strcmp(optarg,"compress")){
                printf("c = %c\n",c);
                printf("optarg = %s\n", optarg);
                cmd->card.acc_type |= 1 << ACC_TYPE_COMPRESS;
            } else{
                fprintf(stderr, "unsupported acc type\n");
            }            
            break;            
        default:
            fprintf(stderr, "unsupport param: %c\n", c);
            break;
        }
        
    }


    if(0 == cmd->card.vendor 
        || 0 == cmd->cmd_type
        || 0 == cmd->card.model 
        || 0 == cmd->card.acc_type 
        || '\0' == cmd->card.vmpath[0] 
        || '\0' == cmd->card.vm_uuid[0]){
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
    
    write(g_handler.cmdsrv_fd, cmd, sizeof(*cmd));
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




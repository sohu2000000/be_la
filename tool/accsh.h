#ifndef __ACCSH_H__
#define __ACCSH_H__


/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   accsh.h
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

#include "../be_la_cmd.h"
#include "../be_la_vendor.h"

typedef struct tag_ACCSH_HANDLE ACCSH_HANDLE;

struct tag_ACCSH_HANDLE
{
    int cmdsrv_fd;    
};

#endif



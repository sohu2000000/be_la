#ifndef __BE_LA_H__
#define __BE_LA_H__


/*****************************************************************
* Copyright (C) 2017 Lenovo Co.,Ltd.*
******************************************************************
* 
* FILENAME:
*   be_la_main.h
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

typedef struct tag_BE_LA_COMM_HANDLE BE_LA_COMM_HANDLE;

struct tag_BE_LA_COMM_HANDLE
{
    int cmd_fd;
    int vseria_fd;
    void** aux;
};

#endif



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

#include <stdio.h>
#include <stdarg.h>

#include "be_la_log.h"

enum log_level g_log_level = BE_LA_LEVEL_DEBUG;


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
void be_la_log_print(int fd,const char*fmt,...)
{
	va_list ap;
	va_start(ap,fmt);

	vdprintf(fd,fmt,ap);
	va_end(ap);
}

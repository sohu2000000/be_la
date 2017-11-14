#include <stdio.h>
#include <stdarg.h>
#include "acc_log.h"

enum log_level g_log_level = ACC_LEVEL_DEBUG;

void acc_log_print(int fd,const char*fmt,...)
{
	va_list ap;
	va_start(ap,fmt);

	vdprintf(fd,fmt,ap);
	va_end(ap);
}

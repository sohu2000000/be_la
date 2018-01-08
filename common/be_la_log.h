#ifndef BE_LA_LOG_H
#define BE_LA_LOG_H

#include <errno.h>
#include <string.h>

enum log_level {
	BE_LA_LEVEL_DEBUG, 
    BE_LA_LEVEL_INFO, 
    BE_LA_LEVEL_LOG, 
    BE_LA_LEVEL_ERROR
};
extern enum log_level g_log_level;

void be_la_log_print(int fd,const char*fmt,...);

#define BE_LA_LOG_INNER(level,fmt,...)\
do{\
		if(level >= g_log_level)\
		{\
			be_la_log_print(1,fmt,##__VA_ARGS__);\
		}\
}while(0)

#define BE_LA_LOG_COMMON(level,fmt,...)\
	BE_LA_LOG_INNER(level,"be_la[%s:%d]: "fmt,__FILE__,__LINE__,##__VA_ARGS__);

#define BE_LA_LOG(fmt,...)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_LOG,fmt,##__VA_ARGS__)

#define BE_LA_ERROR(fmt,...)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_ERROR,fmt,##__VA_ARGS__)

#define BE_LA_DEBUG(fmt,...)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_DEBUG,fmt,##__VA_ARGS__)
	
#define ACC_DEBUG BE_LA_DEBUG
#define ACC_LOG   BE_LA_LOG
#define ACC_ERROR BE_LA_ERROR

#define ACC_PERROR(message)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_ERROR,"%s:%s\n",message,strerror(errno))
#endif

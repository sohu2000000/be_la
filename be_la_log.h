#ifndef BE_LA_LOG_H
#define BE_LA_LOG_H

enum log_level {
	BE_LA_LEVEL_DEBUG, 
    BE_LA_LEVEL_INFO, 
    BE_LA_LEVEL_LOG, 
    BE_LA_LEVEL_ERROR
};
extern enum log_level g_log_level;

void be_la_log_print(int fd,const char*fmt,...);

#define BE_LA_LOG_COMMON(level,fmt,...)\
	do{\
		if(level >= g_log_level)\
		{\
			be_la_log_print(1,"be_la[%s:%d]: "fmt,__FILE__,__LINE__,##__VA_ARGS__);\
		}\
}while(0)

#define BE_LA_LOG(fmt,...)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_LOG,fmt,##__VA_ARGS__)

#define BE_LA_ERROR(fmt,...)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_ERROR,fmt,##__VA_ARGS__)

#define BE_LA_DEBUG(fmt,...)\
	BE_LA_LOG_COMMON(BE_LA_LEVEL_DEBUG,fmt,##__VA_ARGS__)
	
#endif

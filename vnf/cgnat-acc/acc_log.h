#ifndef ACC_LOG_H
#define ACC_LOG_H

enum log_level {
	ACC_LEVEL_DEBUG, ACC_LEVEL_INFO, ACC_LEVEL_LOG, ACC_LEVEL_ERROR
};
extern enum log_level g_log_level;

void acc_log_print(int fd,const char*fmt,...);

#define ACC_LOG_COMMON(level,fmt,...)\
	do{\
		if(level >= g_log_level)\
		{\
			acc_log_print(1,"%s:%d: "fmt,__FILE__,__LINE__,##__VA_ARGS__);\
		}\
}while(0)

#define ACC_LOG(fmt,...)\
	ACC_LOG_COMMON(ACC_LEVEL_LOG,fmt,##__VA_ARGS__)

#define ACC_ERROR(fmt,...)\
	ACC_LOG_COMMON(ACC_LEVEL_ERROR,fmt,##__VA_ARGS__)

#define ACC_DEBUG(fmt,...)\
	ACC_LOG_COMMON(ACC_LEVEL_DEBUG,fmt,##__VA_ARGS__)
#endif

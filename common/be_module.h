/*
 * acc_register.h
 *
 *  Created on: Oct 27, 2017
 *      Author: anlang
 */

#ifndef SRC_COMMON_ACC_MODULE_H_
#define SRC_COMMON_ACC_MODULE_H_

#include <unistd.h>

#define MAKE_SELF_DEAD _exit(1)

#define MAKE_USER_LEVEL(level) 101+level


#define module_init(level,fun,...)\
static __attribute__((constructor(MAKE_USER_LEVEL(level)))) void module_init_ ##fun()\
{\
    if(fun(__VA_ARGS__))\
    {\
        MAKE_SELF_DEAD;\
    }\
}

#define module_destory(level,fun,...)\
static __attribute__((destructor(MAKE_USER_LEVEL(level)))) void module_destroy_ ##fun()\
{\
    if(fun(__VA_ARGS__))\
    {\
        MAKE_SELF_DEAD;\
    }\
}

#endif /* SRC_COMMON_ACC_MODULE_H_ */

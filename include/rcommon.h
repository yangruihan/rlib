#ifndef __R_COMMON_H_
#define __R_COMMON_H_

#include <stdio.h>
#include <stdlib.h>

#include "rplatform.h"

#ifdef R_PLATFORM_WINDOWS
    #ifdef R_DYNAMIC_LINK
        #ifdef R_BUILD
            #define RLIB_API __declspec(dllexport)
        #else
            #define RLIB_API __declspec(dllimport)
        #endif
    #else
        #define RLIB_API extern
    #endif
#else
    #define RLIB_API extern
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef bool
    #define bool int
    #define true 1
    #define false 0
#endif

typedef struct RArray Array;
typedef struct RTable Table;
typedef struct RStack Stack;
typedef struct RLogEvent LogEvent;

#ifdef __cplusplus
}
#endif

#endif // __R_COMMON_H_
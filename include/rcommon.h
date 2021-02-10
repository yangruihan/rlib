#ifndef __R_COMMON_H_
#define __R_COMMON_H_

#include <stdio.h>
#include <stdlib.h>

#ifndef bool
    #define bool int
    #define true 1
    #define false 0
#endif

typedef struct RArray Array;
typedef struct RTable Table;
typedef struct RLogEvent LogEvent;

#endif // __R_COMMON_H_
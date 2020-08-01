#ifndef __R_COMMON_H_
#define __R_COMMON_H_

#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32)
    #define RLIB_WINDOWS 1
#elif defined(__APPLE__)
    #define RLIB_MACOS 1
#endif

#ifndef bool
#define bool int
#define true 1
#define false 0
#endif

#endif // __R_COMMON_H_
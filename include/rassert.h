#ifndef __R_ASSERT_H__
#define __R_ASSERT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcommon.h"
#include "rplatform.h"

#ifdef R_PLATFORM_WINDOWS
    #define DebugBreak(x) __debugbreak()
#elif defined(R_PLATFORM_MACOS)
    #define DebugBreak(x) assert(x)
#else
    #define DebugBreak(x)
#endif

#ifdef R_ENABLE_ASSERT
    #define R_ASSERT(x, ...)  \
        {                      \
            if (!(x))          \
            {                  \
                DebugBreak(x); \
            }                  \
        }
#else
    #define R_ASSERT(x, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif // __R_ASSERT_H__
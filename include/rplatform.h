#ifndef __R_PLATFORM_H__
#define __R_PLATFORM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //define something for Windows (32-bit and 64-bit, this part is common)
    #define R_PLATFORM_WINDOWS
    #ifdef _WIN64
        #define R_PLATFORM_WINDOWS_64
    #else
        #define R_PLATFORM_WINDOWS_32
    #endif
#elif __APPLE__
    #define R_PLATFORM_APPLE
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define R_PLATFORM_IPHONE_SIMULATOR
    #elif TARGET_OS_IPHONE
        #define R_PLATFORM_IPHONE
    #elif TARGET_OS_MAC
        #define R_PLATFORM_MACOS
    #else
        #error "Unknown Apple platform"
    #endif
#elif __linux__
    #define R_PLATFORM_LINUX
#elif __unix__ // all unices not caught above
    #define R_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
    #define R_PLATFORM_POSIX
#else
    #error "Unknown compiler"
#endif

#ifdef __cplusplus
}
#endif

#endif // __R_PLATFORM_H__
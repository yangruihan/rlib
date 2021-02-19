#ifndef __R_LOG_H_
#define __R_LOG_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcommon.h"

struct RLogEvent
{
    va_list     ap;
    const char* fmt;
    const char* file;
    struct tm*  time;
    void*       udata;
    int         line;
    int         level;
};

typedef void (*RLogLogFn) (LogEvent* ev);
typedef void (*RLogLockFn)(bool lock, void* udata);

typedef enum RLogLevel
{
    RLOG_TRACE,
    RLOG_DEBUG,
    RLOG_INFO,
    RLOG_WARN,
    RLOG_ERROR,
    RLOG_FATAL
} LogLevel;

#define RLOG_TRACE(...) rlog_log(RLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define RLOG_DEBUG(...) rlog_log(RLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define RLOG_INFO(...)  rlog_log(RLOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define RLOG_WARN(...)  rlog_log(RLOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define RLOG_ERROR(...) rlog_log(RLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define RLOG_FATAL(...) rlog_log(RLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

RLIB_API const char* rlog_levelString(int level);
RLIB_API void rlog_setLock(RLogLockFn fn, void* udata);
RLIB_API void rlog_setLevel(int level);
RLIB_API void rlog_setQuiet(bool enable);
RLIB_API int  rlog_addCallback(RLogLogFn fn, void* udata, int level);
RLIB_API int  rlog_addFp(FILE* fp, int level);
RLIB_API void rlog_log(int level, const char* file, int line, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // __R_LOG_H_
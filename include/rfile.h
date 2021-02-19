#ifndef __R_FILE_H__
#define __R_FILE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcommon.h"

#include "rlog.h"
#include "rmem.h"

RLIB_API bool readFile(const char* path, /* out | memory alloc*/ char** content, /* out */ int* fileSize);
RLIB_API void combinePath(const char* path, const char* path2, /* out | memory alloc */ char** ret);

#ifdef __cplusplus
}
#endif

#endif // __R_FILE_H__
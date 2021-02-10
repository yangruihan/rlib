#ifndef __R_FILE_H__
#define __R_FILE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcommon.h"

#include "rlog.h"
#include "rmem.h"

bool readFile(const char* path, /* out */ char** content, /* out */ int* fileSize);
void combinePath(const char* path, const char* path2, /* out */ char** ret);

#ifdef __cplusplus
}
#endif

#endif // __R_FILE_H__
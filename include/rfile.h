#ifndef __R_FILE_H__
#define __R_FILE_H__

#include "rcommon.h"

#include "rlog.h"
#include "rmem.h"

bool readFile(const char* path, /* out */ char** content, /* out */ int* fileSize);

#endif // __R_FILE_H__
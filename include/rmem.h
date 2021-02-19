#ifndef __R_MEM_H_
#define __R_MEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcommon.h"

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(type, p) \
    reallocate(p, sizeof(type), 0)

#define GROW_CAPACITY(cap) \
    ((cap) < 8 ? 8 : (cap) * 2)

#define GROW_ARRAY(prev, type, oldCnt, newCnt) \
    (type*)reallocate(prev, sizeof(type) * (oldCnt), sizeof(type) * (newCnt))

#define FREE_ARRAY(type, p, oldCnt) \
    reallocate(p, sizeof(type) * (oldCnt), 0)

RLIB_API void* reallocate(void* prev, size_t oldSize, size_t newSize);

#ifdef __cplusplus
}
#endif

#endif
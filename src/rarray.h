#ifndef __R_ARRAY_H_
#define __R_ARRAY_H_

#include "rcommon.h"

struct RArray
{
    void* data;
    int count;
    int capacity;
    int elemSize;
};

#define ARR_INIT(array, type) array_init(array, 8, sizeof(type))
#define ARR_INIT_CAP(array, type, cap) array_init(array, cap, sizeof(type))
#define ARR_PUSH(array, type, value) \
    do \
    { \
        type _ = value; \
        array_push(array, &_); \
    } while(false)
#define ARR_SET(array, type, index, value) \
    do \
    { \
        type _ = value; \
        array_set(array, index, &_); \
    } while(false)

void array_init(Array* array, int cap, int elemSize);
void array_free(Array* array);

void array_push(Array* array, void* value);
bool array_pop(Array* array, void* value);
bool array_set(Array* array, int index, void* value);
bool array_get(Array* array, int index, void* value);
bool array_remove(Array* array, int index);

#endif
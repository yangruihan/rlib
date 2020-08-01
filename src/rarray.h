#ifndef __R_ARRAY_H_
#define __R_ARRAY_H_

#include "rcommon.h"

typedef struct sArray
{
    void* data;
    int count;
    int capacity;
    int elemSize;
} Array;

#define ARR_INIT(array, cap, type) array_init(array, cap, sizeof(type))
void array_init(Array* array, int cap, int elemSize);
void array_free(Array* array);

void array_push(Array* array, void* value);
bool array_pop(Array* array, void* value);
bool array_set(Array* array, int index, void* value);
bool array_get(Array* array, int index, void* value);
bool array_remove(Array* array, int index);

#endif
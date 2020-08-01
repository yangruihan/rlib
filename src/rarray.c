#include "rarray.h"

#include <memory.h>

#include "rmem.h"

#define ALLOCATE_ARR(array, count) \
    GROW_ARRAY(array->data, char, array->capacity * array->elemSize, count * array->elemSize)

#define ARR_DATA_OFFSET(array, count) \
    ((char*)((array)->data) + (array)->elemSize * (count))

#define ARR_SET_DATA(array, index, value) \
    memcpy(ARR_DATA_OFFSET((array), (index)), (value), (array)->elemSize)

#define ARR_GET_DATA(array, index, value) \
    memcpy((value), ARR_DATA_OFFSET((array), (index)), (array)->elemSize)

void array_init(Array* array, int cap, int elemSize)
{
    array->data = NULL;
    array->elemSize = elemSize;
    array->data = ALLOCATE_ARR(array, cap);
    memset(array->data, 0, cap * array->elemSize);
    array->count = 0;
    array->capacity = cap;
}

void array_free(Array* array)
{
    FREE_ARRAY(char, array->data, array->capacity * array->elemSize);
    array->data = NULL;
    array->capacity = 0;
    array->count = 0;
    array->elemSize = 0;
}

void array_push(Array* array, void* value)
{
    if (array->count == array->capacity)
    {
        int newCap = GROW_CAPACITY(array->capacity);
        ALLOCATE_ARR(array, newCap);
    }

    ARR_SET_DATA(array, array->count, value);
    array->count++;
}

bool array_pop(Array* array, void* value)
{
    if (array->count == 0)
        return false;

    ARR_GET_DATA(array, array->count - 1, value);
    return true;
}

bool array_set(Array* array, int index, void* value)
{
    if (index < 0 || index >= array->count)
        return false;

    ARR_SET_DATA(array, index, value);
    return true;
}

bool array_get(Array* array, int index, void* value)
{
    if (index < 0 || index >= array->count)
        return false;

    ARR_GET_DATA(array, index, value);
    return true;
}

bool array_remove(Array* array, int index)
{
    if (index < 0 || index >= array->count)
        return false;

    memset(ARR_DATA_OFFSET(array, index), 0, array->elemSize);
    return true;
}
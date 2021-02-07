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
    if (array == NULL)
        return;

    array->data = NULL;
    array->elemSize = elemSize;
    array->data = ALLOCATE_ARR(array, cap);
    memset(array->data, 0, cap * array->elemSize);
    array->count = 0;
    array->capacity = cap;
}

void array_free(Array* array)
{
    if (array == NULL)
        return;

    FREE_ARRAY(char, array->data, array->capacity * array->elemSize);
    array->data = NULL;
    array->capacity = 0;
    array->count = 0;
    array->elemSize = 0;
}

void array_push(Array* array, void* value)
{
    if (array == NULL)
        return;

    if (array->count == array->capacity)
    {
        int newCap = GROW_CAPACITY(array->capacity);
        array->data = ALLOCATE_ARR(array, newCap);
        array->capacity = newCap;
    }

    array->count++;
    array_set(array, array->count - 1, value);
}

bool array_pop(Array* array, void* value)
{
    if (array == NULL)
        return false;

    if (array->count == 0)
        return false;

    if (value != NULL)
        ARR_GET_DATA(array, array->count - 1, value);

    array->count--;
    return true;
}

bool array_set(Array* array, int index, void* value)
{
    if (array == NULL)
        return false;

    if (index < 0 || index >= array->count)
        return false;

    if (value != NULL)
    {
        ARR_SET_DATA(array, index, value);
    }
    else
    {
        memset(ARR_DATA_OFFSET(array, index), 0, array->elemSize);
    }
    
    return true;
}

bool array_get(Array* array, int index, void* value)
{
    if (array == NULL)
        return false;

    if (index < 0 || index >= array->count)
        return false;

    ARR_GET_DATA(array, index, value);
    return true;
}

bool array_remove(Array* array, int index)
{
    if (array == NULL)
        return false;

    if (index < 0 || index >= array->count)
        return false;

    memcpy(ARR_DATA_OFFSET(array, index), 
           ARR_DATA_OFFSET(array, index + 1), 
           array->elemSize * (array->count - index - 1));

    array->count--;
    return true;
}

bool array_removeRange(Array* array, int index, int count)
{
    if (array == NULL)
        return false;

    if (index < 0 || index >= array->count)
        return false;

    if (array->count - index < count)
        return false;

    if (count < 0)
        return false;

    array->count -= count;

    if (index < array->count)
        memcpy(ARR_DATA_OFFSET(array, index), ARR_DATA_OFFSET(array, index + count), (array->count - index) * array->elemSize);

    memset(ARR_DATA_OFFSET(array, array->count), 0, count * array->elemSize);

    return true;
}

void array_clear(Array* array)
{
    if (array == NULL)
        return;

    array->count = 0;
}

bool array_eq(Array* a, Array* b)
{
    if (a == b && a != NULL)
        return true;

    if (a == NULL || b == NULL)
        return false;

    if (a->count != b->count)
        return false;

    if (a->elemSize != b->elemSize)
        return false;

    for (size_t i = 0; i < a->count; i++)
    {
        if (memcmp(ARR_DATA_OFFSET(a, i), 
                   ARR_DATA_OFFSET(b, i),
                   a->elemSize) != 0)
            return false;
    }

    return true;
}

int array_indexOf(Array* array, void* value, int elemSize)
{
    if (array == NULL)
        return -1;

    if (array->elemSize != elemSize)
        return -1;

    for (size_t i = 0, len = array->count; i < len; i++)
    {
        if (memcmp(value, ARR_DATA_OFFSET(array, i), elemSize) == 0)
            return i;
    }

    return -1;
}

int array_lastIndexOf(Array* array, void* value, int elemSize)
{
    if (array == NULL)
        return -1;

    if (array->elemSize != elemSize)
        return -1;

    for (int i = array->count - 1; i >= 0; i--)
    {
        if (memcmp(value, ARR_DATA_OFFSET(array, i), elemSize) == 0)
            return i;
    }

    return -1;
}

bool array_contains(Array* array, void* value, int elemSize)
{
    return array_indexOf(array, value, elemSize) >= 0;
}

bool array_insert(Array* array, int index, void* value)
{
    if (array == NULL)
        return false;

    if (index < 0 || index > array->count)
        return false;
    
    if (array->count == array->capacity)
    {
        int newCap = GROW_CAPACITY(array->capacity);
        array->data = ALLOCATE_ARR(array, newCap);
        array->capacity = newCap;
    }

    if (index < array->count)
        memcpy(ARR_DATA_OFFSET(array, index + 1), ARR_DATA_OFFSET(array, index), (array->count - index) * array->elemSize);

    array->count++;
    array_set(array, index, value);
    return true;
}
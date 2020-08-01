#include "rmem.h"

void* reallocate(void* prev, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(prev);
        return NULL;
    }

    return realloc(prev, newSize);
}
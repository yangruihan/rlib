#ifndef __R_TABLE_H_
#define __R_TABLE_H_

#include "rcommon.h"

typedef struct REntry
{
    uint32_t key;
    void*    value;
} Entry;

struct RTable
{
    int   count;
    int   capacity;
    int   elemSize;
    void* entries;
};

void table_init(Table* t, int elemSize);
void table_free(Table* t);
bool table_get(Table* t, uint32_t key, void* val);
bool table_set(Table* t, uint32_t key, void* val);
bool table_del(Table* t, uint32_t key);


#endif // __R_TABLE_H_
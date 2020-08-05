#ifndef __R_TABLE_H_
#define __R_TABLE_H_

#include "rcommon.h"

struct RTable
{
    int   count;
    int   capacity;
    int   elemSize;
    void* entries;

    int   _entrySize;   // entrySize = elemSize + sizeof(uint32_t)
    bool  _zeroSetFlag; // whether set zero
    char* _zeroValue;   // record zero hash value 
    char* _emptyMem;    // empty mem to compare
};

typedef void (*TableIterFunc) (uint32_t key, void* value);

#define TABLE_INIT(t, type) table_init(t, sizeof(type))
#define TABLE_SET(t, type, key, value) \
    do \
    { \
        type _ = value; \
        table_set(t, key, &_); \
    } while(false)

void table_init(Table* t, int elemSize);
void table_free(Table* t);
bool table_get(Table* t, uint32_t key, void* val);
bool table_set(Table* t, uint32_t key, void* val);
bool table_del(Table* t, uint32_t key);
void table_keys(Table* t, uint32_t* keys);
void table_iter(Table* t, TableIterFunc func);

#endif // __R_TABLE_H_

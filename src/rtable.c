#include "rtable.h"

#include <memory.h>

#include "rmem.h"

#define TABLE_MAX_LOAD 0.75

#define KEY(entry) ((uint32_t*)(entry))
#define VALUE(entry) ((char*)(entry) + s_u32size)

#define TABLE_DATA_OFFSET(table, index) \
    (((char*)(table->entries)) + (table->_entrySize * index))

#define ENTRIES_DATA_OFFSET(entries, elemSize, index) \
    (((char*)(entries)) + ((elemSize + s_u32size) * index))

#define ENTRY_VALUE_IS(entry, memBuf, len) \
    (memcmp(VALUE((entry)), (memBuf), (len)) == 0)

static int s_u32size = sizeof(uint32_t);

static char* findEntry(char* entries, int elemSize,
                     int cap, uint32_t key, 
                     char* emptyMem, char* zeroValue,
                     /* out */ bool* found)
{
    uint32_t index = key % cap;
    char* tombstone = NULL;

    for(;;)
    {
        char* entry = ENTRIES_DATA_OFFSET(entries, elemSize, index);
        uint32_t entry_key = *KEY(entry);

        if (entry_key == 0)
        {
            if (ENTRY_VALUE_IS(entry, emptyMem, elemSize))
            {
                if (found != NULL) *found = false;
                return tombstone != NULL ? tombstone : entry;
            }
            else
            {
                if (tombstone == NULL) 
                    tombstone = entry;
            }
        }
        else if (entry_key == key)
        {
            if (found != NULL) *found = true;
            return entry;
        }

        index = (index + 1) % cap;
    }
}

static void adjustCap(Table* t, int cap)
{
    int sumMemLen = cap * t->_entrySize;
    char* entries = ALLOCATE(char, sumMemLen);
    memset(entries, 0, sumMemLen);

    t->count = t->_zeroSetFlag ? 1 : 0;
    for (size_t i = 0; i < t->capacity; i++)
    {
        char* entry = TABLE_DATA_OFFSET(t, i);
        if (*KEY(entry) == 0 && ENTRY_VALUE_IS(entry, t->_emptyMem, t->elemSize)) continue;

        char* dest = findEntry(entries, t->elemSize, cap, *KEY(entry),
                               t->_emptyMem, t->_zeroValue, NULL);
        *KEY(dest) = *KEY(entry);
        memcpy(VALUE(dest), VALUE(entry), t->elemSize);
        t->count++;
    }

    if (t->entries)
        FREE_ARRAY(char, (char*)t->entries, t->capacity * t->_entrySize);

    t->entries = entries;
    t->capacity = cap;
}

void table_init(Table* t, int elemSize)
{
    if (t == NULL)
        return;

    t->count = 0;
    t->capacity = 0;
    t->elemSize = elemSize;
    t->entries = NULL;

    t->_entrySize = s_u32size + elemSize;
    t->_zeroSetFlag = false;
    t->_zeroValue = ALLOCATE(char, t->elemSize);
    t->_emptyMem = ALLOCATE(char, t->elemSize);

    memset(t->_zeroValue, 0, t->elemSize);
    memset(t->_emptyMem, 0, t->elemSize);
}

void table_free(Table* t)
{
    if (t == NULL)
        return;

    if (t->entries)
        FREE_ARRAY(char, t->entries, t->capacity * t->_entrySize);

    if (t->_zeroValue)
        FREE_ARRAY(char, t->_zeroValue, t->elemSize);

    if (t->_emptyMem)
        FREE_ARRAY(char, t->_emptyMem, t->elemSize);

    t->count = 0;
    t->capacity = 0;
    t->elemSize = 0;
    t->entries = NULL;

    t->_entrySize = 0;
    t->_zeroSetFlag = false;
    t->_zeroValue = NULL;
    t->_emptyMem = NULL;
}

bool table_get(Table* t, uint32_t key, void* val)
{
    if (t == NULL || t->count == 0)
        return false;

    if (key == 0 && t->_zeroSetFlag)
    {
        memcpy(val, t->_zeroValue, t->elemSize);
        return true;
    }

    bool found;
    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key,
                            t->_emptyMem, t->_zeroValue, &found);
    if (!found) return false;

    if (val)
        memcpy(val, VALUE(entry), t->elemSize);

    return true;
}

bool table_set(Table* t, uint32_t key, void* val)
{
    if (t == NULL)
        return false;

    if (key == 0)
    {
        if (!t->_zeroSetFlag)
        {
            t->count++;
            t->_zeroSetFlag = true;
        }

        memcpy(t->_zeroValue, val, t->elemSize);
        return true;
    }

    if (t->count + 1 > t->capacity * TABLE_MAX_LOAD)
    {
        size_t cap = GROW_CAPACITY(t->capacity);
        adjustCap(t, cap);
    }

    bool found;
    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key,
                            t->_emptyMem, t->_zeroValue, &found);

    *KEY(entry) = key;
    memcpy(VALUE(entry), val, t->elemSize);

    if (!found) t->count++;

    return !found;
}

bool table_del(Table* t, uint32_t key)
{
    if (t == NULL || t->count == 0)
        return false;

    if (key == 0)
    {
        if (t->_zeroSetFlag)
        {
            t->_zeroSetFlag = false;
            t->count--;
            return true;
        }

        return false;
    }

    bool found;
    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key,
                            t->_emptyMem, t->_zeroValue, &found);

    if (found)
    {
        *KEY(entry) = 0;
        t->count--;
    }

    return found;
}

void table_keys(Table* t, uint32_t* keys)
{
    if (t == NULL || keys == NULL)
        return;

    size_t index = 0;

    if (t->_zeroSetFlag)
    {
        keys[index] = 0;
        index++;
    }

    for (size_t i = 0; i < t->capacity; i++)
    {
        char* entry = TABLE_DATA_OFFSET(t, i);
        if (*KEY(entry) == 0) continue;

        keys[index] = *KEY(entry);
        index++;
    }
}

void table_iter(Table* t, TableIterFunc func)
{
    if (t == NULL || func == NULL)
        return;

    if (t->_zeroSetFlag)
        func(0, t->_zeroValue);

    for (size_t i = 0; i < t->capacity; i++)
    {
        char* entry = TABLE_DATA_OFFSET(t, i);
        if (*KEY(entry) == 0) continue;

        func(*KEY(entry), VALUE(entry));
    }
}

bool table_eq(Table* a, Table* b)
{
    if (a == b && a != NULL)
        return true;

    if (a == NULL || b == NULL)
        return false;

    if (a->count != b->count)
        return false;

    if (a->elemSize != b->elemSize)
        return false;

    if (a->_zeroSetFlag != b->_zeroSetFlag)
        return false;

    if (memcmp(a->_zeroValue, b->_zeroValue, a->elemSize) != 0)
        return false;

    char* bvalue = ALLOCATE(char, a->elemSize);
    for (size_t i = 0; i < a->capacity; i++)
    {
        char* entry = TABLE_DATA_OFFSET(a, i);
        uint32_t key = *KEY(entry);
        if (key == 0) continue;

        if (table_get(b, key, bvalue))
        {
            if (memcmp(VALUE(entry), bvalue, a->elemSize) != 0)
                goto FALSE;
        }
        else
        {
            goto FALSE;
        }
    }

    FREE_ARRAY(char, bvalue, a->elemSize);
    return true;

    FALSE:
    FREE_ARRAY(char, bvalue, a->elemSize);
    return false;
}
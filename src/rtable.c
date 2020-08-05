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

#define ENTRY_IS_NO_USED(entry, t) \
    (*KEY((entry)) == 0 && (!ENTRY_VALUE_IS((entry), (t)->_zeroValue, (t)->elemSize)))

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

        if (entry_key == key)
        {
            if (key == 0)
            {
                if (ENTRY_VALUE_IS(entry, zeroValue, elemSize))
                {
                    if (found != NULL) *found = true;
                    return entry;
                }
            }
            else
            {
                if (found != NULL) *found = true;
                return entry;
            }
        }
        
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

        index = (index + 1) % cap;
    }
}

static void adjustCap(Table* t, int cap)
{
    int sumMemLen = cap * t->_entrySize;
    char* entries = ALLOCATE(char, sumMemLen);
    memset(entries, 0, sumMemLen);

    t->count = 0;
    for (size_t i = 0; i < t->capacity; i++)
    {
        char* entry = TABLE_DATA_OFFSET(t, i);
        if (ENTRY_IS_NO_USED(entry, t)) continue;

        char* dest = findEntry(entries, t->elemSize, cap, *KEY(entry),
                               t->_emptyMem, t->_zeroValue, NULL);
        *KEY(dest) = *KEY(entry);
        *VALUE(dest) = *VALUE(dest);
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
    t->_zeroValue = ALLOCATE(char, t->elemSize);
    t->_emptyMem = ALLOCATE(char, t->elemSize);

    memset(t->_zeroValue, 1, t->elemSize);
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
    t->_zeroValue = NULL;
    t->_emptyMem = NULL;
}

bool table_get(Table* t, uint32_t key, void* val)
{
    if (t == NULL || t->count == 0)
        return false;

    bool found;
    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key,
                            t->_emptyMem, t->_zeroValue, &found);
    if (!found) return false;

    if (val != NULL)
        memcpy(val, VALUE(entry), t->elemSize);

    return true;
}

bool table_set(Table* t, uint32_t key, void* val)
{
    if (t == NULL)
        return false;

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

    // set zero value
    if (key == 0)
        memcpy(t->_zeroValue, val, t->elemSize);

    if (!found) t->count++;

    return !found;
}

bool table_del(Table* t, uint32_t key)
{
    if (t == NULL || t->count == 0)
        return false;

    bool found;
    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key,
                            t->_emptyMem, t->_zeroValue, &found);

    if (found)
        *KEY(entry) = 0;

    return found;
}

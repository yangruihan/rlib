#include "rtable.h"

#include <memory.h>

#include "rmem.h"

#define TABLE_MAX_LOAD 0.75

#define KEY(entry) ((uint32_t*)(entry))
#define VALUE(entry) ((char*)(entry) + s_u32size)

#define TABLE_DATA_OFFSET(table, index) \
    (((char*)(table->entries)) + (table->entrySize * index))

#define ENTRIES_DATA_OFFSET(entries, elemSize, index) \
    (((char*)(entries)) + ((elemSize + s_u32size) * index))

static int s_u32size = sizeof(uint32_t); 

static void* findEntry(char* entries, int elemSize, int cap, uint32_t key)
{
    uint32_t index = key % cap;
    char* tombstone = NULL;

    for(;;)
    {
        char* entry = ENTRIES_DATA_OFFSET(entries, elemSize, index);
        uint32_t entry_key = *KEY(entry);

        if (entry_key == 0)
        {
            if (*VALUE(entry) == 1)
                return tombstone != NULL ? tombstone : entry;
            else
                if (tombstone == NULL) tombstone = entry;
        }
        else if (entry_key == key)
        {
            return entry;
        }

        index = (index + 1) % cap;
    }
}

static void adjustCap(Table* t, int cap)
{
    int sumMemLen = cap * t->entrySize;
    char* entries = ALLOCATE(char, sumMemLen);
    for (int i = 0; i < cap; i++)
    {
        char* entry = ENTRIES_DATA_OFFSET(entries, t->elemSize, i);
        memset(entry, 0, t->elemSize);
        *VALUE(entry) = 1;
    }

    t->count = 0;
    for (size_t i = 0; i < t->capacity; i++)
    {
        char* entry = TABLE_DATA_OFFSET(t, i);
        if (*KEY(entry) == 0 && *VALUE(entry) == 1) continue;

        char* dest = findEntry(entries, t->elemSize, cap, *KEY(entry));
        *KEY(dest) = *KEY(entry);
        *VALUE(dest) = *VALUE(dest);
        t->count++;
    }

    if (t->entries)
        FREE_ARRAY(char, (char*)t->entries, t->capacity * t->entrySize);

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
    t->entrySize = s_u32size + elemSize;
    t->entries = NULL;
}

void table_free(Table* t)
{
    if (t == NULL)
        return;

    if (t->entries)
        FREE_ARRAY(char, t->entries, t->capacity * t->entrySize);

    table_init(t, 0);
}

bool table_get(Table* t, uint32_t key, void* val)
{
    if (t == NULL || t->count == 0)
        return false;

    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key);
    if (*KEY(entry) == 0) return false;

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

    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key);
    bool isNewKey = *KEY(entry) == 0;

    *KEY(entry) = key;
    memcpy(VALUE(entry), val, t->elemSize);

    if (isNewKey) t->count++;

    return isNewKey;
}

bool table_del(Table* t, uint32_t key)
{
    if (t == NULL || t->count == 0)
        return false;

    char* entry = findEntry(t->entries, t->elemSize, t->capacity, key);

    *KEY(entry) = 0;

    return true;
}

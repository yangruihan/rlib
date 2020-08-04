#include "rtable.h"

#include <memory.h>

#include "rmem.h"

#define TABLE_MAX_LOAD 0.75

#define TABLE_DATA_OFFSET(table, index) \
    ((Entry*)(((char*)(table->entries)) + (table->elemSize * index)))

#define ENTRY_IS_NULL(e) \
    ((e)->key == 0 && (e)->value == NULL)

static Entry* findEntry(char* entries, int elemSize, int cap, uint32_t key)
{
    uint32_t index = key % cap;
    Entry* tombstone = NULL;

    for(;;)
    {
        Entry* entry = (Entry*)(entries + (index * elemSize));

        if (entry->key == 0)
        {
            if (entry->value == NULL)
                return tombstone != NULL ? tombstone : entry;
            else
                if (tombstone == NULL) tombstone = entry;
        }
        else if (entry->key == key)
        {
            return entry;
        }

        index = (index + 1) % cap;
    }
}

static void adjustCap(Table* t, int cap)
{
    char* entries = ALLOCATE(char, cap * t->elemSize);
    memset(entries, 0, cap * t->elemSize);

    t->count = 0;
    for (size_t i = 0; i < t->capacity; i++)
    {
        Entry* entry = TABLE_DATA_OFFSET(t, i);
        if (ENTRY_IS_NULL(entry)) continue;

        Entry* dest = findEntry(entries, t->elemSize, cap, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        t->count++;
    }

    if (t->entries)
        FREE_ARRAY(char, (char*)t->entries, t->capacity * t->elemSize);

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
}

void table_free(Table* t)
{
    if (t == NULL)
        return;

    if (t->entries)
        FREE_ARRAY(char, t->entries, t->capacity * t->elemSize);

    table_init(t, 0);
}

bool table_get(Table* t, uint32_t key, void* val)
{
    if (t == NULL || t->count == 0)
        return false;

    Entry* entry = findEntry(t->entries, t->elemSize, t->capacity, key);
    if (ENTRY_IS_NULL(entry)) return false;

    memcpy(val, &entry->value, t->elemSize);
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

    Entry* entry = findEntry(t->entries, t->elemSize, t->capacity, key);
    bool isNewKey = ENTRY_IS_NULL(entry);

    entry->key = key;
    memcpy(&entry->value, val, t->elemSize);

    if (isNewKey) t->count++;

    return isNewKey;
}

bool table_del(Table* t, uint32_t key)
{
    if (t == NULL || t->count == 0)
        return false;

    Entry* entry = findEntry(t->entries, t->elemSize, t->capacity, key);

    entry->key = 0;
    memset(entry->value, 0, t->elemSize);

    return true;
}

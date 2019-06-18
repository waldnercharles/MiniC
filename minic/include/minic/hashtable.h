#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "minic/allocator.h"
#include "minic/int.h"

typedef struct Hashtable Hashtable;

void
hashtable_init(Hashtable *table,
               Allocator *allocator,
               usize element_size,
               u32 capacity);

void
hashtable_free(Hashtable *table);

void
hashtable_add(Hashtable *table, u64 key, void *value);

void
hashtable_remove(Hashtable *table, u64 key);

void
hashtable_clear(Hashtable *table);

void *
hashtable_get(Hashtable *table, u64 key);

void
hashtable_set(Hashtable *table, u64 key, void *value);

u32
hashtable_count(Hashtable *table);

u64 *
hashtable_keys(Hashtable *table);

void *
hashtable_values(Hashtable *table);

Iterator
hashtable_keys_iterator(Hashtable *table, usize index);

Iterator
hashtable_values_iterator(Hashtable *table, usize index);

#endif

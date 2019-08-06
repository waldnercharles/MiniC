#include "minic/allocator.h"
#include "minic/array.h"
#include "minic/assert.h"
#include "minic/hashtable.h"
#include "minic/int.h"
#include "minic/math.h"
#include "minic/memory.h"

typedef struct
{
    u32 key_hash;
    u32 item_index;

    u32 count;
} HashtableSlot;

typedef struct
{
    u64 *key;
    u32 *slot_index;
    void *value;

    u32 count;
    u32 capacity;
    usize element_size;
} HashtableItems;

struct Hashtable
{
    Allocator *allocator;

    u32 slots_capacity;
    HashtableSlot *slots;

    HashtableItems items;
};

static u32
hashtable_hash(u64 key)
{
    key = (~key) + (key << 18);
    key = key ^ (key >> 31);
    key = key * 21;
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);

    assert(key > 0);
    return cast(u32, key);
}

static s32
hashtable_get_slot_index(Hashtable *table, u64 key)
{
    u32 slots_capacity_mask = table->slots_capacity - 1;
    u32 hash = hashtable_hash(key);

    u32 hash_index = hash & slots_capacity_mask;
    u32 count = table->slots[hash_index].count;

    u32 slot_index = hash_index;

    while (count > 0)
    {
        u32 slot_hash = table->slots[slot_index].key_hash;
        if (slot_hash)
        {
            u32 probe_slot_index = slot_hash & slots_capacity_mask;
            if (probe_slot_index == hash_index)
            {
                assert(count > 0);
                --count;

                u64 item_key =
                    table->items.key[table->slots[slot_index].item_index];

                if (slot_hash == hash && item_key == key)
                {
                    return cast(s32, slot_index);
                }
            }
        }

        slot_index = (slot_index + 1) & slots_capacity_mask;
    }

    return -1;
}

static void
hashtable_expand_slots(Hashtable *table)
{
    HashtableSlot *old_slots = table->slots;
    u32 old_capacity = table->slots_capacity;

    table->slots_capacity <<= 1;
    usize slots_bytes = table->slots_capacity * sizeof(HashtableSlot);

    table->slots =
        cast(HashtableSlot *, allocator_alloc(table->allocator, slots_bytes));

    assert(table->slots != NULL);
    mem_set(table->slots, 0, slots_bytes);

    u32 slots_capacity_mask = table->slots_capacity - 1;

    for (u32 i = 0; i < old_capacity; ++i)
    {
        u32 hash = old_slots[i].key_hash;
        u32 item_index = old_slots[i].item_index;

        if (hash)
        {
            u32 hash_index = hash & slots_capacity_mask;
            u32 slot_index = hash_index;

            while (table->slots[slot_index].key_hash)
            {
                slot_index = (slot_index + 1) & slots_capacity_mask;
            }

            table->slots[slot_index].key_hash = hash;
            table->slots[slot_index].item_index = item_index;
            table->items.slot_index[item_index] = slot_index;

            ++(table->slots[hash_index].count);
        }
    }

    allocator_free(table->allocator, old_slots);
}

static void
hashtable_expand_items(Hashtable *table)
{
    u64 *old_key = table->items.key;
    void *old_value = table->items.value;
    u32 *old_slot_index = table->items.slot_index;

    table->items.capacity <<= 1;

    usize items_bytes = table->items.capacity *
                        (sizeof(u64) + sizeof(u32) + table->items.element_size);

    table->items.key =
        cast(u64 *, allocator_alloc(table->allocator, items_bytes));

    assert(table->items.key != NULL);

    table->items.slot_index =
        reinterpret_cast<u32 *>(table->items.key + table->items.capacity);

    table->items.value =
        cast(void *, table->items.slot_index + table->items.capacity);

    u32 count = table->items.count;
    mem_copy(table->items.key, old_key, count * sizeof(u64));
    mem_copy(table->items.slot_index, old_slot_index, count * sizeof(u32));
    mem_copy(table->items.value, old_value, count * table->items.element_size);

    allocator_free(table->allocator, old_key);
}

void
hashtable_init(Hashtable *table,
               Allocator *allocator,
               usize element_size,
               u32 capacity)
{
    assert(table != NULL);
    assert(allocator != NULL);
    assert(element_size != 0);
    assert(capacity != 0);

    table->allocator = allocator;

    table->slots_capacity = next_pow2(capacity + (capacity >> 1));
    usize slots_bytes = table->slots_capacity * sizeof(HashtableSlot);

    table->slots =
        static_cast<HashtableSlot *>(allocator_alloc(allocator, slots_bytes));

    assert(table->slots != NULL);
    mem_set(table->slots, 0, slots_bytes);

    table->items.count = 0u;
    table->items.capacity = next_pow2(capacity);
    table->items.element_size = element_size;

    usize items_bytes = table->items.capacity *
                        (sizeof(u64) + sizeof(u32) + table->items.element_size);

    table->items.key = cast(u64 *, allocator_alloc(allocator, items_bytes));

    assert(table->items.key != NULL);
    mem_set(table->items.key, 0, items_bytes);

    table->items.slot_index =
        reinterpret_cast<u32 *>(table->items.key + table->items.capacity);

    table->items.value =
        cast(void *, table->items.slot_index + table->items.capacity);
}

void
hashtable_free(Hashtable *table)
{
    assert(table != NULL);
    assert(table->allocator != NULL);
    allocator_free(table->allocator, table->items.key);
    allocator_free(table->allocator, table->slots);
}

void
hashtable_add(Hashtable *table, u64 key, void *value)
{
    assert(table != NULL);

    u32 expand_threshold = table->slots_capacity - (table->slots_capacity / 3u);
    if (table->items.count >= expand_threshold)
    {
        hashtable_expand_slots(table);
    }

    u32 slots_capacity_mask = table->slots_capacity - 1;
    u32 hash = hashtable_hash(key);

    u32 hash_index = hash & slots_capacity_mask;
    u32 count = table->slots[hash_index].count;

    u32 slot_index = hash_index;
    u32 free_slot_index = slot_index;

    while (count > 0)
    {
        u32 slot_hash = table->slots[slot_index].key_hash;

        if (slot_hash == 0 && table->slots[free_slot_index].key_hash != 0)
        {
            free_slot_index = slot_index;
        }

        u32 probe_slot_index = slot_hash & slots_capacity_mask;
        if (probe_slot_index == hash_index)
        {
            --count;
        }

        slot_index = (slot_index + 1) & slots_capacity_mask;
    }

    slot_index = free_slot_index;

    while (table->slots[slot_index].key_hash != 0)
    {
        slot_index = (slot_index + 1) * slots_capacity_mask;
    }

    while (table->items.count >= table->items.capacity)
    {
        hashtable_expand_items(table);
    }

    assert(table->slots[slot_index].key_hash == 0);
    assert(hash != 0);

    table->slots[slot_index].key_hash = hash;
    table->slots[slot_index].item_index = table->items.count;
    ++(table->slots[hash_index].count);

    void *dst_value =
        cast(void *,
             cast(u8 *, table->items.value) +
                 (table->items.count * table->items.element_size));

    mem_copy(dst_value, value, table->items.element_size);
    table->items.key[table->items.count] = key;
    table->items.slot_index[table->items.count] = slot_index;
    ++(table->items.count);
}

void
hashtable_remove(Hashtable *table, u64 key)
{
    s32 slot_index = hashtable_get_slot_index(table, key);
    assert(slot_index >= 0);

    u32 slot_capacity_mask = table->slots_capacity - 1;
    u32 hash = table->slots[slot_index].key_hash;

    u32 hash_index = hash & slot_capacity_mask;
    assert(hash > 0);

    --(table->slots[hash_index].count);

    table->slots[slot_index].key_hash = 0u;

    u32 item_index = table->slots[slot_index].item_index;
    u32 last_index = table->items.count - 1u;

    if (item_index != last_index)
    {
        u64 last_key = table->items.key[last_index];
        u32 last_slot_index = table->items.slot_index[last_index];

        table->items.key[item_index] = last_key;
        table->items.slot_index[item_index] = last_slot_index;

        void *dst_value = cast(void *,
                               cast(u8 *, table->items.value) +
                                   (item_index * table->items.element_size));
        void *src_value = cast(void *,
                               cast(u8 *, table->items.value) +
                                   (last_index * table->items.element_size));

        mem_copy(dst_value, src_value, table->items.element_size);

        table->slots[last_slot_index].item_index = item_index;
    }

    --(table->items.count);
}

void
hashtable_clear(Hashtable *table)
{
    table->items.count = 0;
    mem_set(table->slots, 0, table->slots_capacity * sizeof(HashtableSlot));
}

void *
hashtable_get(Hashtable *table, u64 key)
{
    s32 slot_index = hashtable_get_slot_index(table, key);
    if (slot_index < 0)
    {
        return NULL;
    }

    u32 item_index = table->slots[slot_index].item_index;

    void *value = cast(void *,
                       cast(u8 *, table->items.value) +
                           (item_index * table->items.element_size));

    assert(value != NULL);

    return value;
}

void
hashtable_set(Hashtable *table, u64 key, void *value)
{
    s32 slot_index = hashtable_get_slot_index(table, key);
    if (slot_index >= 0)
    {
        u32 item_index = table->slots[slot_index].item_index;

        void *dst_value = cast(void *,
                               cast(u8 *, table->items.value) +
                                   (item_index * table->items.element_size));

        mem_copy(dst_value, value, table->items.element_size);
    }
    else
    {
        hashtable_add(table, key, value);
    }
}

u32
hashtable_count(Hashtable *table)
{
    assert(table != NULL);

    return table->items.count;
}

u64 *
hashtable_keys(Hashtable *table)
{
    assert(table != NULL);

    return table->items.key;
}

void *
hashtable_values(Hashtable *table)
{
    assert(table != NULL);

    return table->items.value;
}

Iterator
hashtable_keys_iterator(Hashtable *table, usize index)
{
    assert(table != NULL);
    assert(table->items.key != NULL);
    assert(table->items.count > index);

    Iterator iterator;
    iterator_init(&iterator, table->items.key + index, sizeof(u64));

    return iterator;
}

Iterator
hashtable_values_iterator(Hashtable *table, usize index)
{
    assert(table != NULL);
    assert(table->items.value != NULL);
    assert(table->items.count > index);

    Iterator iterator;
    iterator_init(&iterator,
                  cast(u8 *, table->items.value) +
                      (index * table->items.element_size),
                  table->items.element_size);

    return iterator;
}

#include "minic/allocator_default.h"
#include "minic/hashtable.h"
#include "minic/int.h"
#include "minic/io.h"
#include "minic/iterator.h"
#include "minic/rand.h"

s32
main()
{
    Allocator allocator;
    allocator_init_default(&allocator);

    Random rand;
    rand_seed(&rand, 0x92385611ull);

    Hashtable hashtable;
    hashtable_init(&hashtable, &allocator, sizeof(u32), 32u);

    for (u32 i = 0; i < 10; i++)
    {
        u32 tmp = (i + 1) * (i + 1);
        u64 key = rand_next_u64(&rand);
        io_printf("Adding: %llu, %u\n", key, tmp);
        hashtable_add(&hashtable, key, &tmp);
    }

    Iterator keys = hashtable_keys_iterator(&hashtable, 0);
    Iterator values = hashtable_values_iterator(&hashtable, 0);

    u32 remove_count = (rand_next_u64(&rand) & 3) + 1;
    for (u32 i = 0; i < remove_count; i++)
    {
        iterator_advance(&keys, 2);
        u64 key = *cast(u64 *, iterator_current(&keys));
        io_printf("Removing: %llu\n", key);
        hashtable_remove(&hashtable, key);
    }

    keys = hashtable_keys_iterator(&hashtable, 0);
    u32 count = hashtable_count(&hashtable);
    for (u32 i = 0; i < count; i++)
    {
        u64 key = *cast(u64 *, iterator_next(&keys));
        u32 value = *cast(u32 *, iterator_next(&values));
        io_printf("%llu: %u, %u\n",
                  key,
                  value,
                  *cast(u32 *, hashtable_get(&hashtable, key)));
    }

    return 0;
}

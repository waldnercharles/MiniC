#include "win32/windows.h"

#include "clib/allocator_default.h"
#include "clib/array.h"
#include "clib/assert.h"
#include "clib/int.h"
#include "clib/io.h"
#include "clib/rand.h"
#include "clib/timer.h"

s32
main()
{
    Allocator allocator;
    allocator_default_init(&allocator);

    Array arr;
    array_init(&arr, &allocator, sizeof(u32));

    for (u32 i = 0; i < 10; i++)
    {
        array_push_front(&arr, &i);
    }

    for (u32 i = 0; i < 10; i++)
    {
        __printf("%i ", *(u32 *)array_get(&arr, i));
    }

    array_free(&arr);

    /*struct array_u32 *arr;
    arr = allocator_alloc(&allocator, sizeof(arr));

    array_init(arr, &allocator);

    array_reserve(arr, 10);
    assert(arr->capacity == 10);

    array_reserve_pow2(arr, 10);
    assert(arr->capacity == 16);

    arr->length = arr->capacity;
    for (int i = 0; i < arr->capacity; i++)
    {
        array_set(arr, i, i);
        arr->data[i] = i;
    }

    __printf("%i", array_get(arr, 6));*/

    return 0;
}
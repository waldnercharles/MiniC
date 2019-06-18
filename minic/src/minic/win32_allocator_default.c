#include <win32/io.h>

#include "minic/allocator_default.h"
#include "minic/assert.h"
#include "minic/int.h"

static void *
allocator_alloc_default(void *internal_allocator, usize size)
{
    assert(internal_allocator != NULL);
    assert(size > 0);

    return HeapAlloc(internal_allocator, HEAP_ZERO_MEMORY, size);
}

static void
allocator_free_default(void *internal_allocator, void *block)
{
    assert(internal_allocator != NULL);
    assert(block != NULL);

    HeapFree(internal_allocator, 0, block);
}

static void *
allocator_realloc_default(void *internal_allocator, void *block, usize size)
{
    assert(internal_allocator != NULL);
    assert(block != NULL);
    assert(size > 0);

    return HeapReAlloc(internal_allocator, HEAP_ZERO_MEMORY, block, size);
}

void
allocator_init_default(Allocator *allocator)
{
    assert(allocator != NULL);

    void *internal_allocator = GetProcessHeap();

    assert(internal_allocator != NULL);

    allocator_init(allocator,
                   internal_allocator,
                   allocator_alloc_default,
                   allocator_realloc_default,
                   allocator_free_default);
}

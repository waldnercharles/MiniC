#include <win32/io.h>

#include "minic/allocator_default.h"
#include "minic/assert.h"
#include "minic/int.h"

static void *
allocator_default_alloc(void *internal_allocator, usize size)
{
    assert(internal_allocator != NULL);
    assert(size > 0);

    return HeapAlloc(internal_allocator, HEAP_ZERO_MEMORY, size);
}

static void
allocator_default_free(void *internal_allocator, void *block)
{
    assert(internal_allocator != NULL);
    assert(block != NULL);

    HeapFree(internal_allocator, 0, block);
}

static void *
allocator_default_realloc(void *internal_allocator, void *block, usize size)
{
    assert(internal_allocator != NULL);
    assert(block != NULL);
    assert(size > 0);

    return HeapReAlloc(internal_allocator, HEAP_ZERO_MEMORY, block, size);
}

void
allocator_default_init(Allocator *allocator)
{
    assert(allocator != NULL);

    allocator->alloc = allocator_default_alloc;
    allocator->realloc = allocator_default_realloc;
    allocator->free = allocator_default_free;
    allocator->internal_allocator = GetProcessHeap();

    assert(allocator->internal_allocator != NULL);
}

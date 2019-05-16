#include "minic/allocator.h"
#include "minic/assert.h"

inline void *
allocator_alloc(Allocator *allocator, size_t size)
{
    assert(allocator != NULL);
    assert(allocator->alloc != NULL);
    assert(size > 0);

    return allocator->alloc(allocator->internal_allocator, size);
}

inline void *
allocator_realloc(Allocator *allocator, void *block, size_t size)
{
    assert(allocator != NULL);
    assert(allocator->alloc != NULL);
    assert(allocator->realloc != NULL);
    assert(size > 0);

    return block != NULL
               ? allocator->realloc(allocator->internal_allocator, block, size)
               : allocator->alloc(allocator->internal_allocator, size);
}

inline void
allocator_free(Allocator *allocator, void *block)
{
    assert(allocator != NULL);
    assert(allocator->free != NULL);

    if (block != NULL)
    {
        allocator->free(allocator->internal_allocator, block);
    }
}
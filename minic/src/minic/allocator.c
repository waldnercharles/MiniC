#include "minic/allocator.h"
#include "minic/assert.h"

struct Allocator
{
    allocator_alloc_f *alloc;
    allocator_realloc_f *realloc;
    allocator_free_f *free;

    void *internal_allocator;
};

void
allocator_init(Allocator *allocator,
               void *internal_allocator,
               allocator_alloc_f *alloc,
               allocator_realloc_f *realloc,
               allocator_free_f *free)
{
    assert(allocator != NULL);

    allocator->alloc = alloc;
    allocator->realloc = realloc;
    allocator->free = free;

    allocator->internal_allocator = internal_allocator;
}

inline void *
allocator_alloc(Allocator *allocator, usize size)
{
    assert(allocator != NULL);
    assert(allocator->alloc != NULL);
    assert(size > 0);

    return allocator->alloc(allocator->internal_allocator, size);
}

inline void *
allocator_realloc(Allocator *allocator, void *block, usize size)
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

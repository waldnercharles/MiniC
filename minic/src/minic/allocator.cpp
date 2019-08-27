#include "minic/allocator.h"
#include "minic/assert.h"

void allocator_init(Allocator *allocator,
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

template <typename T>
T *memory_alloc(Allocator *allocator, usize count)
{
    assert(allocator != NULL);
    return cast(T *,
                allocator->alloc(allocator->internal_allocator,
                                 sizeof(T) * count));
}

template <typename T>
T *memory_realloc(Allocator *allocator, T *address, usize count)
{
    assert(allocator != NULL);
    return cast(T *,
                allocator->realloc(allocator->internal_allocator,
                                   address,
                                   sizeof(T) * count));
}

template <typename T>
void memory_free(Allocator *allocator, T *address)
{
    assert(allocator != NULL);
    allocator->free(allocator->internal_allocator, address);
}

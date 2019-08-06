#include "minic/allocator.h"
#include "minic/assert.h"

// void *allocator_alloc(Allocator *allocator, usize size)
//{
//    assert(allocator != NULL);
//    return allocator->alloc(size);
//}
//
// template <typename T>
// T *allocator_alloc(Allocator *allocator)
//{
//    assert(allocator != NULL);
//    return cast(T *, allocator->alloc(sizeof(T)));
//}
//
// template <typename T>
// T *allocator_alloc(Allocator *allocator, usize count)
//{
//    assert(allocator != NULL);
//    return cast(T *, allocator->alloc(sizeof(T) * count));
//}
//
// void *allocator_realloc(Allocator *allocator, void *ptr, usize size)
//{
//    assert(allocator != NULL);
//    return allocator->realloc(ptr, size);
//}
//
template <typename T>
T *memory_realloc(Allocator *allocator, usize count)
{
    assert(allocator != NULL);
    return cast(T *, allocator->alloc(sizeof(T) * count));
}

template <typename T>
T *memory_realloc(Allocator *allocator, T *address, usize count)
{
    assert(allocator != NULL);
    return cast(T *, allocator->realloc(address, sizeof(T) * count));
}

template <typename T>
void memory_free(Allocator *allocator, T *address)
{
    assert(allocator != NULL);
    allocator->free(address);
}

#include "minic/allocator_default.h"
#include "minic/assert.h"
#include "minic/int.h"
#include "minic/log.h"

//#include <Windows.h>
#include <win32/io.h>

struct CAllocator
{
    HANDLE handle;
};

static void *c_allocator_alloc(void *internal_allocator, usize bytes)
{
    CAllocator *allocator = cast(CAllocator *, internal_allocator);

    assert(allocator != NULL);
    assert(allocator->handle != NULL);

    assert(bytes > 0);

    return HeapAlloc(allocator->handle, HEAP_ZERO_MEMORY, bytes);
}

static void *c_allocator_realloc(void *internal_allocator,
                                 void *address,
                                 usize bytes)
{
    CAllocator *allocator = cast(CAllocator *, internal_allocator);

    assert(allocator != NULL);
    assert(allocator->handle != NULL);

    assert(bytes > 0);

    if (address == NULL)
    {
        return HeapAlloc(allocator->handle, HEAP_ZERO_MEMORY, bytes);
    }
    else
    {
        return HeapReAlloc(allocator->handle, HEAP_ZERO_MEMORY, address, bytes);
    }
}

static void c_allocator_free(void *internal_allocator, void *address)
{
    CAllocator *allocator = cast(CAllocator *, internal_allocator);
    assert(allocator != NULL);
    assert(allocator->handle != NULL);

    if (address != NULL)
    {
        HeapFree(allocator->handle, 0, address);
    }
    else
    {
        log_debug("Attempted to free NULL memory.");
    }
}

void c_allocator_init(Allocator *allocator)
{
    assert(allocator != NULL);

    HANDLE handle = GetProcessHeap();
    assert(handle != NULL);

    CAllocator *self =
        cast(CAllocator *,
             HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeof(CAllocator)));

    self->handle = handle;

    allocator_init(allocator,
                   self,
                   c_allocator_alloc,
                   c_allocator_realloc,
                   c_allocator_free);
}

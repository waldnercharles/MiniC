#include <win32/io.h>

#include "minic/allocator_default.h"
#include "minic/assert.h"
#include "minic/int.h"
#include "minic/log.h"

CAllocator::CAllocator()
{
    this->handle = GetProcessHeap();

    assert(this->handle != NULL);
}

void *CAllocator::alloc(usize bytes)
{
    assert(this->handle != NULL);
    assert(bytes > 0);

    return HeapAlloc(this->handle, HEAP_ZERO_MEMORY, bytes);
}

void *CAllocator::realloc(void *address, usize bytes)
{
    assert(this->handle != NULL);
    assert(bytes > 0);

    if (address == NULL)
    {
        return HeapAlloc(this->handle, HEAP_ZERO_MEMORY, bytes);
    }
    else
    {
        return HeapReAlloc(this->handle, HEAP_ZERO_MEMORY, address, bytes);
    }
}

void CAllocator::free(void *address)
{
    assert(this->handle != NULL);
    if (address != NULL)
    {
        HeapFree(this->handle, 0, address);
    }
    else
    {
        log_debug("Attempted to free NULL memory.");
    }
}

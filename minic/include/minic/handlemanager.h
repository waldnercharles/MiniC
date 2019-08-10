#pragma once

#include "minic/array.h"
#include "minic/handle.h"
#include "minic/int.h"

struct HandleManager
{
public:
    HandleManager(Allocator *allocator);

    Handle create();
    void destroy(Handle handle);

    bool is_valid(Handle handle) const;

private:
    u32 index_count;

    Array<u32> free_indices;
    Array<u32> generations;

    void reserve(usize count);

    HandleManager(const HandleManager &) = delete;
    HandleManager &operator=(const HandleManager &) = delete;
};

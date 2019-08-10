#pragma once

#include "minic/allocator.h"
#include "minic/int.h"

union Handle
{
public:
    Handle();
    Handle(u64 raw);
    Handle(u32 index, u32 generation);

    u64 get_id() const;
    u32 get_index() const;
    u32 get_generation() const;

private:
    struct
    {
        u32 index;
        u32 generation;
    };
    u64 id;

    friend struct HandleManager;
};

#include "minic/array.h"
#include "minic/assert.h"
#include "minic/handle.h"

Handle::Handle()
{
    this->id = 0;
}

Handle::Handle(u64 raw)
{
    this->id = raw;
}

Handle::Handle(u32 index, u32 generation)
{
    this->index = index;
    this->generation = generation;
}

u64 Handle::get_id() const
{
    return this->id;
}

u32 Handle::get_index() const
{
    return this->index;
}

u32 Handle::get_generation() const
{
    return this->generation;
}

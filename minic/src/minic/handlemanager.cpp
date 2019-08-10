#include "minic/assert.h"
#include "minic/handlemanager.h"

HandleManager::HandleManager(Allocator *allocator)
    : index_count(1), free_indices(allocator), generations(allocator)
{
}

Handle HandleManager::create()
{
    u32 index;
    u32 generation;

    if (this->free_indices.is_empty())
    {
        index = this->index_count++;
        this->reserve(this->index_count);
        generation = this->generations[index] = 1;
    }
    else
    {
        index = this->free_indices.back();
        this->free_indices.pop_back();
        generation = this->generations[index];
    }

    return { index, generation };
}

void HandleManager::destroy(Handle handle)
{
    assert(this->is_valid(handle));

    this->generations[handle.index]++;
    this->free_indices.push_back(handle.index);
}

bool HandleManager::is_valid(Handle handle) const
{
    return (handle.index != 0) &&
           (handle.index < this->generations.get_count()) &&
           (this->generations[handle.index] == handle.get_generation());
}

void HandleManager::reserve(usize reserve_count)
{
    const usize current_count = this->generations.get_count();

    assert(reserve_count > current_count);
    this->generations.insert(current_count, reserve_count - current_count, 0);
}

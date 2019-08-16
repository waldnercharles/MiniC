#include "minic/array.h"
#include "minic/assert.h"
#include "minic/handle.h"

void handle_manager_init(HandleManager *handle_manager, Allocator *allocator)
{
    assert(handle_manager != NULL);

    handle_manager->index_count = 1;

    array_init(&handle_manager->free_indices, allocator);
    array_init(&handle_manager->generations, allocator);
}

void handle_manager_free(HandleManager *handle_manager)
{
    assert(handle_manager != NULL);

    array_free(&handle_manager->free_indices);
    array_free(&handle_manager->generations);
}

static void handle_allocate(HandleManager *handle_manager, usize reserve_count)
{
    assert(handle_manager != NULL);
    const usize current_count = handle_manager->generations.count;

    assert(reserve_count > current_count);
    array_insert(&handle_manager->generations,
                 current_count,
                 reserve_count - current_count,
                 0u);
}

Handle handle_create(HandleManager *handle_manager)
{
    assert(handle_manager != NULL);

    u32 index;
    u32 generation;

    if (array_is_empty(&handle_manager->free_indices))
    {
        index = handle_manager->index_count++;
        handle_allocate(handle_manager, handle_manager->index_count);
        generation = handle_manager->generations[index] = 1;
    }
    else
    {
        index = array_back(&handle_manager->free_indices);
        array_pop_back(&handle_manager->free_indices);
        generation = handle_manager->generations[index];
    }

    return { { index, generation } };
}

void handle_destroy(HandleManager *handle_manager, Handle handle)
{
    assert(handle_manager != NULL);
    assert(handle_validate(handle_manager, handle));

    handle_manager->generations[handle.index]++;
    array_push_back(&handle_manager->free_indices, handle.index);
}

bool handle_validate(HandleManager *handle_manager, Handle handle)
{
    assert(handle_manager != NULL);

    return (handle.index != 0) &&
           (handle.index < handle_manager->generations.count) &&
           (handle_manager->generations[handle.index] == handle.generation);
}

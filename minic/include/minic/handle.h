#pragma once

#include "minic/allocator.h"
#include "minic/int.h"

struct HandleManager
{
    u32 index_count;

    Array<u32> free_indices;
    Array<u32> generations;
};

union Handle
{
    struct
    {
        u32 index;
        u32 generation;
    };
    u64 id;
};

void handle_manager_init(HandleManager *handle_manager, Allocator *allocator);
void handle_manager_free(HandleManager *handle_manager);

Handle handle_create(HandleManager *handle_manager);
void handle_destroy(HandleManager *handle_manager, Handle handle);
bool handle_validate(HandleManager *handle_manager, Handle handle);

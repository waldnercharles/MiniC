#include "minic/array.h"
#include "minic/assert.h"
#include "minic/handle.h"

static const u32 HANDLE_DEFAULT_CAPACITY = 32u;
static const u32 HANDLE_FREELIST_UNASSIGNED = 0xFFFFFFFFu;

typedef union
{
    struct
    {
        u32 key;
        u32 generation;
    };
    u64 raw;
} Handle;

struct HandleManager
{
    Allocator *allocator;

    Handle *handles;
    u32 handles_count;
    u32 handles_capacity;

    u32 freelist_head;
    u32 freelist_tail;
};

static void
handle_expand(HandleManager *manager)
{
    manager->handles_capacity <<= 1;

    manager->handles =
        allocator_realloc(manager->allocator,
                          manager->handles,
                          sizeof(Handle) * manager->handles_capacity);
    assert(manager->handles != NULL);
}

overload void
handle_init(HandleManager *manager, Allocator *allocator, u32 capacity)
{
    assert(manager != NULL);
    assert(allocator != NULL);
    assert(capacity > 0);

    manager->allocator = allocator;

    manager->handles_count = 0u;
    manager->handles_capacity = capacity;
    manager->handles = allocator_alloc(allocator, sizeof(Handle) * capacity);

    manager->freelist_head = HANDLE_FREELIST_UNASSIGNED;
    manager->freelist_tail = HANDLE_FREELIST_UNASSIGNED;
}

overload void
handle_init(HandleManager *manager, Allocator *allocator)
{
    handle_init(manager, allocator, HANDLE_DEFAULT_CAPACITY);
}

void
handle_free(HandleManager *manager)
{
    allocator_free(manager->allocator, manager->handles);
}

u32
handle_create(HandleManager *manager, u32 key)
{
    u32 handle_index;

    if (manager->handles_count < manager->handles_capacity)
    {
        handle_index = manager->handles_count;
        manager->handles[handle_index].generation = 1u;
        manager->handles_count++;
    }
    else if (manager->freelist_head != HANDLE_FREELIST_UNASSIGNED)
    {
        handle_index = manager->freelist_head;

        if (manager->freelist_head == manager->freelist_tail)
        {
            manager->freelist_tail =
                manager->handles[manager->freelist_head].key;
        }

        manager->freelist_head = manager->handles[manager->freelist_head].key;
    }
    else
    {
        handle_expand(manager);

        handle_index = manager->handles_count;
        manager->handles[handle_index].generation = 1u;
        manager->handles_count++;
    }

    manager->handles[handle_index].key = key;

    return handle_index;
}

void
handle_dispose(HandleManager *manager, u32 handle_index)
{
    if (manager->freelist_tail == HANDLE_FREELIST_UNASSIGNED)
    {
        assert(manager->freelist_head == HANDLE_FREELIST_UNASSIGNED);

        manager->freelist_head = handle_index;
        manager->freelist_tail = handle_index;
    }
    else
    {
        manager->handles[manager->freelist_tail].key = handle_index;
        manager->freelist_tail = handle_index;
    }

    manager->handles[handle_index].generation++;
    manager->handles[handle_index].key = HANDLE_FREELIST_UNASSIGNED;
}

inline u64
handle_get(HandleManager *manager, u32 handle_index)
{
    assert(handle_index < manager->handles_count);
    return manager->handles[handle_index].raw;
}

inline u32
handle_key(HandleManager *manager, u32 handle_index)
{
    Handle handle = { .raw = handle_get(manager, handle_index) };
    return handle.key;
}

inline u32
handle_generation(HandleManager *manager, u32 handle_index)
{
    Handle handle = { .raw = handle_get(manager, handle_index) };
    return handle.generation;
}

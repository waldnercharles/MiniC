#pragma once

#include "minic/allocator.h"
#include "minic/int.h"

typedef struct HandleManager HandleManager;

overload void
handle_init(HandleManager *manager, Allocator *allocator, u32 capacity);

overload void
handle_init(HandleManager *manager, Allocator *allocator);

void
handle_free(HandleManager *manager);

u32
handle_create(HandleManager *manager, u32 key);

void
handle_dispose(HandleManager *manager, u32 handle);

inline u64
handle_get(HandleManager *manager, u32 handle_index);

inline u32
handle_key(HandleManager *manager, u32 handle_index);

inline u32
handle_generation(HandleManager *manager, u32 handle_index);

#pragma once

#include "minic/int.h"

typedef void *(allocator_alloc_f)(void *allocator, usize size);
typedef void *(allocator_realloc_f)(void *allocator, void *block, usize size);
typedef void(allocator_free_f)(void *allocator, void *block);

typedef struct Allocator Allocator;

void
allocator_init(Allocator *allocator,
               void *internal_allocator,
               allocator_alloc_f *alloc,
               allocator_realloc_f *realloc,
               allocator_free_f *free);

void *
allocator_alloc(Allocator *allocator, usize size);

void *
allocator_realloc(Allocator *allocator, void *block, usize size);

void
allocator_free(Allocator *allocator, void *block);

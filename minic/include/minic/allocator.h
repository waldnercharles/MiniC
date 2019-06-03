#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "minic/int.h"

typedef void *(allocator_alloc_f)(void *allocator, usize size);
typedef void *(allocator_realloc_f)(void *allocator, void *block, usize size);
typedef void(allocator_free_f)(void *allocator, void *block);

typedef struct
{
    allocator_alloc_f *alloc;
    allocator_realloc_f *realloc;
    allocator_free_f *free;

    void *internal_allocator;
} Allocator;

void *
allocator_alloc(Allocator *allocator, usize size);

void *
allocator_realloc(Allocator *allocator, void *block, usize size);

void
allocator_free(Allocator *allocator, void *block);

#endif

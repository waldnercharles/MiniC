#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "clib/int.h"

typedef void *(allocator_alloc_f)(void *allocator, size_t size);
typedef void *(allocator_realloc_f)(void *allocator, void *block, size_t size);
typedef void(allocator_free_f)(void *allocator, void *block);

typedef struct
{
    allocator_alloc_f *alloc;
    allocator_realloc_f *realloc;
    allocator_free_f *free;

    void *internal_allocator;
} Allocator;

void *
allocator_alloc(Allocator *allocator, size_t size);

void *
allocator_realloc(Allocator *allocator, void *block, size_t size);

void
allocator_free(Allocator *allocator, void *block);

#endif
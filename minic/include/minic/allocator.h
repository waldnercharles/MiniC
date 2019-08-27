#pragma once

#include "minic/int.h"

typedef void *(allocator_alloc_f)(void *allocator, usize size);
typedef void *(allocator_realloc_f)(void *allocator, void *block, usize size);
typedef void(allocator_free_f)(void *allocator, void *block);

struct Allocator
{
private:
    void *internal_allocator;

    allocator_alloc_f *alloc;
    allocator_realloc_f *realloc;
    allocator_free_f *free;

    friend void allocator_init(Allocator *allocator,
                               void *internal_allocator,
                               allocator_alloc_f *alloc,
                               allocator_realloc_f *realloc,
                               allocator_free_f *free);

    template <typename T>
    friend T *memory_alloc(Allocator *allocator, usize count);
    template <typename T>
    friend T *memory_realloc(Allocator *allocator, T *address, usize count);
    template <typename T>
    friend void memory_free(Allocator *allocator, T *address);
};

void allocator_init(Allocator *allocator,
                    void *internal_allocator,
                    allocator_alloc_f *alloc,
                    allocator_realloc_f *realloc,
                    allocator_free_f *free);

template <typename T>
T *memory_alloc(Allocator *allocator, usize count);

template <typename T>
T *memory_realloc(Allocator *allocator, T *address, usize count);

template <typename T>
void memory_free(Allocator *allocator, T *address);

// void *allocator_alloc(Allocator *allocator, usize size);
//
// void *allocator_realloc(Allocator *allocator, void *ptr, usize size);
//
// void allocator_free(Allocator *allocator, void *ptr);
//
// template <typename T>
// T *allocator_alloc(Allocator *allocator);
//
// template <typename T>
// T *allocator_alloc(Allocator *allocator, usize count);
//
// template <typename T>
// T *allocator_realloc(Allocator *allocator, void *ptr);
//
// template <typename T>
// T *allocator_realloc(Allocator *allocator, void *ptr, usize count);

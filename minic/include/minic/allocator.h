#pragma once

#include "minic/int.h"

struct Allocator
{
public:
    virtual ~Allocator() = default;

protected:
    virtual void *alloc(usize bytes) = 0;
    virtual void *realloc(void *address, usize bytes) = 0;
    virtual void free(void *address) = 0;

    template <typename T>
    friend T *memory_alloc(Allocator *allocator, usize count);

    template <typename T>
    friend T *memory_realloc(Allocator *allocator, T *address, usize count);

    template <typename T>
    friend void memory_free(Allocator *allocator, T *address);
};

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

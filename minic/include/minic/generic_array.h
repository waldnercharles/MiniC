#pragma once
#include "minic/allocator.h"
#include "minic/assert.h"
#include "minic/int.h"
#include "minic/math.h"
#include "minic/memory.h"

static const u32 ARRAY_DEFAULT_CAPACITY = 32u;

#define array_define(T, TArray)                                                \
    struct TArray                                                              \
    {                                                                          \
        Allocator *allocator;                                                  \
                                                                               \
        usize count;                                                           \
        usize capacity;                                                        \
        T *data;                                                               \
    };                                                                         \
                                                                               \
    overload bool array_reserve(struct TArray *arr, usize capacity);           \
    overload void array_init(struct TArray *arr,                               \
                             Allocator *allocator,                             \
                             usize capacity);                                  \
    overload void array_init(struct TArray *arr, Allocator *allocator);        \
    overload void array_free(struct TArray *arr);                              \
    overload void array_copy(struct TArray *dst, struct TArray *src);          \
    overload void array_move(struct TArray *dst, struct TArray *src);          \
    overload bool array_reserve_pow2(struct TArray *arr, usize capacity);      \
    overload T array_get(struct TArray *arr, usize index);                     \
    overload void array_set(struct TArray *arr, usize index, T value);         \
    overload void array_insert(struct TArray *arr, usize index, T value);      \
    overload void array_push_back(struct TArray *arr, T value);                \
    overload void array_push_front(struct TArray *arr, T value);               \
    overload T *array_front(struct TArray *arr);                               \
    overload T *array_back(struct TArray *arr);                                \
    overload usize array_count(struct TArray *arr);                            \
    overload usize array_capacity(struct TArray *arr);                         \
                                                                               \
    overload bool array_reserve(struct TArray *arr, usize capacity)            \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(capacity != 0);                                                 \
                                                                               \
        if (capacity > arr->capacity)                                          \
        {                                                                      \
            void *new_data = allocator_realloc(arr->allocator,                 \
                                               arr->data,                      \
                                               capacity * sizeof(T));          \
                                                                               \
            assert(new_data != NULL);                                          \
                                                                               \
            if (new_data == NULL)                                              \
            {                                                                  \
                return false;                                                  \
            }                                                                  \
                                                                               \
            arr->data = new_data;                                              \
            arr->capacity = capacity;                                          \
        }                                                                      \
                                                                               \
        return true;                                                           \
    }                                                                          \
    overload void array_init(struct TArray *arr,                               \
                             Allocator *allocator,                             \
                             usize capacity)                                   \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(allocator != NULL);                                             \
        assert(capacity > 0);                                                  \
                                                                               \
        arr->allocator = allocator;                                            \
        arr->count = 0;                                                        \
        arr->capacity = 0;                                                     \
        arr->data = NULL;                                                      \
                                                                               \
        array_reserve(arr, capacity);                                          \
    }                                                                          \
                                                                               \
    overload void array_init(struct TArray *arr, Allocator *allocator)         \
    {                                                                          \
        array_init(arr, allocator, ARRAY_DEFAULT_CAPACITY);                    \
    }                                                                          \
                                                                               \
    overload void array_free(struct TArray *arr)                               \
    {                                                                          \
        assert(arr != NULL);                                                   \
                                                                               \
        allocator_free(arr->allocator, cast(void *, arr->data));               \
        mem_set(arr, 0, sizeof(*arr));                                         \
    }                                                                          \
                                                                               \
    overload void array_copy(struct TArray *dst, struct TArray *src)           \
    {                                                                          \
        assert(src != NULL);                                                   \
        assert(dst != NULL);                                                   \
        assert(src->data != NULL);                                             \
                                                                               \
        if (dst->data != NULL)                                                 \
        {                                                                      \
            array_free(dst);                                                   \
        }                                                                      \
                                                                               \
        array_init(dst, src->allocator, src->capacity);                        \
        mem_copy(dst->data, src->data, src->count * sizeof(T));                \
    }                                                                          \
                                                                               \
    overload void array_move(struct TArray *dst, struct TArray *src)           \
    {                                                                          \
        assert(src != NULL);                                                   \
        assert(dst != NULL);                                                   \
        assert(src->data != NULL);                                             \
                                                                               \
        if (dst->data != NULL)                                                 \
        {                                                                      \
            array_free(dst);                                                   \
        }                                                                      \
                                                                               \
        *dst = *src;                                                           \
        mem_set(src, 0, sizeof(*src));                                         \
    }                                                                          \
                                                                               \
    overload bool array_reserve_pow2(struct TArray *arr, usize capacity)       \
    {                                                                          \
        return array_reserve(arr, next_pow2(capacity));                        \
    }                                                                          \
                                                                               \
    overload T array_get(struct TArray *arr, usize index)                      \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(index < arr->count);                                            \
                                                                               \
        return arr->data[index];                                               \
    }                                                                          \
                                                                               \
    overload void array_set(struct TArray *arr, usize index, T value)          \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(index < arr->count);                                            \
                                                                               \
        arr->data[index] = value;                                              \
    }                                                                          \
                                                                               \
    overload void array_insert(struct TArray *arr, usize index, T value)       \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(index <= arr->count);                                           \
                                                                               \
        if (arr->count >= arr->capacity)                                       \
        {                                                                      \
            array_reserve_pow2(arr, arr->count + 1);                           \
        }                                                                      \
                                                                               \
        T *offset = arr->data + index;                                         \
        mem_move(offset + 1, offset, sizeof(T) * (arr->count - index));        \
                                                                               \
        arr->data[index] = value;                                              \
        arr->count++;                                                          \
    }                                                                          \
                                                                               \
    overload void array_push_back(struct TArray *arr, T value)                 \
    {                                                                          \
        assert(arr != NULL);                                                   \
                                                                               \
        if (arr->count >= arr->capacity)                                       \
        {                                                                      \
            array_reserve_pow2(arr, arr->count + 1);                           \
        }                                                                      \
                                                                               \
        arr->count++;                                                          \
        array_set(arr, arr->count - 1, value);                                 \
    }                                                                          \
                                                                               \
    overload void array_push_front(struct TArray *arr, T value)                \
    {                                                                          \
        array_insert(arr, 0, value);                                           \
    }                                                                          \
                                                                               \
    overload T *array_front(struct TArray *arr)                                \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(arr->data != NULL);                                             \
                                                                               \
        return arr->data;                                                      \
    }                                                                          \
                                                                               \
    overload T *array_back(struct TArray *arr)                                 \
    {                                                                          \
        assert(arr != NULL);                                                   \
        assert(arr->data != NULL);                                             \
                                                                               \
        return arr->data + (arr->count - 1);                                   \
    }                                                                          \
                                                                               \
    overload usize array_count(struct TArray *arr) { return arr->count; }      \
    overload usize array_capacity(struct TArray *arr)                          \
    {                                                                          \
        return arr->capacity;                                                  \
    }                                                                          \
                                                                               \
    typedef struct TArray TArray

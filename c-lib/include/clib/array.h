#ifndef ARRAY_H
#define ARRAY_H

#include "clib/int.h"
#include "clib/allocator.h"
#include "clib/iterator.h"

#define ARRAY_DEFAULT_CAPACITY 2

typedef struct
{
    Allocator *allocator;

    u32 size;
    u32 capacity;
    u32 element_size;
    void *data;
} Array;

overload void
array_init(Array *arr, Allocator *allocator, u32 capacity, u32 element_size);

overload void
array_init(Array *arr, Allocator *allocator, u32 element_size);

void
array_free(Array *arr);

void
array_copy(Array *dst, Array *src);

void
array_move(Array *dst, Array *src);

bool
array_reserve(Array *arr, u32 capacity);

bool
array_reserve_pow2(Array *arr, u32 capacity);

void
array_push_back(Array *Arr, void *value);

void
array_push_front(Array *Arr, void *value);

void
array_insert(Array *arr, u32 index, void *value);

void
array_set(Array *arr, u32 index, void *value);

void
array_pop_back(Array *arr);

void
array_pop_front(Array *arr);

overload void
array_remove(Array *arr, u32 index);

overload void
array_remove(Array *arr, Iterator *iterator);

void
array_clear(Array *arr);

void *
array_get(Array *arr, u32 index);

void *
array_front(Array *arr);

void *
array_back(Array *arr);

Iterator
array_begin(Array *arr);

Iterator
array_end(Array *arr);

Iterator
array_iterator(Array *arr, u32 index);

u32
array_iterator_index(Array *arr, Iterator *iterator);

void *
_array_offset(Array *arr, u32 index);

// static inline bool
// array_empty(struct array *arr)
//{
//    assert(arr != NULL);
//
//    return arr->size == 0;
//}
//
// static inline void *
// array_get_ptr(struct array *arr, u32 index)
//{
//    assert(arr != NULL);
//    assert(arr->data != NULL);
//    assert(arr->element_size > 0);
//
//    assert(index >= 0);
//    assert(index < arr->size);
//
//    return cast(u8 *, arr->data) + (index * arr->element_size);
//}
//
// static inline void
// array_set(struct array *arr, u32 index, const void *value)
//{
//    assert(arr != NULL);
//    assert(arr->element_size > 0);
//
//    assert(value != NULL);
//
//    mem_copy(array_get_ptr(arr, index), value, arr->element_size);
//}
//
// overload static inline void
// array_add_range(struct array *arr, const void *values, u32 n)
//{
//    (void)arr, (void)values, (void)n;
//}

//#define	array_add(arr, val)                                                    \
//	array_
//
//#define array_add_range(arr, vals, len)

/* clang-format on */

// #define array_first(arr) (arr)->data[0]
// #define array_last(arr) (arr)->data[(arr)->length - 1]

// void
// array_add_range(struct array *array, const T *values, u32 length)
//{
//    u32 new_length = array->length + length;
//    if (array->capacity < new_length)
//    {
//        array_reserve_pow2(array, new_length);
//    }
//}
//
// bool
// array_reserve_pow2(struct array *array, u32 capacity)
//{
//    return array_reserve(array, u32_next_pow2(capacity));
//}
//
//
// bool
// array_grow_by(struct array *array, u32 count)
//{
//    u32 capacity = array->capacity == 0 ? 1 : array->capacity << 1;
//    return allocator_realloc(array->allocator, array->data, capacity);
//}

#endif
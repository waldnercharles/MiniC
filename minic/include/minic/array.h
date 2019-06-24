#pragma once

#include "minic/int.h"
#include "minic/allocator.h"
#include "minic/iterator.h"

typedef struct Array Array;

overload void
array_init(Array *arr,
           Allocator *allocator,
           usize element_size,
           usize capacity);

overload void
array_init(Array *arr, Allocator *allocator, usize element_size);

void
array_free(Array *arr);

void
array_copy(Array *dst, Array *src);

void
array_move(Array *dst, Array *src);

bool
array_reserve(Array *arr, usize capacity);

bool
array_reserve_pow2(Array *arr, usize capacity);

void
array_push_back(Array *Arr, void *value);

void
array_push_front(Array *Arr, void *value);

void
array_insert(Array *arr, usize index, void *value);

void
array_set(Array *arr, usize index, void *value);

void
array_pop_back(Array *arr);

void
array_pop_front(Array *arr);

void
array_iterator_remove(Array *arr, Iterator *iterator);

void
array_remove(Array *arr, usize index);

void
array_clear(Array *arr);

void *
array_get(Array *arr, usize index);

void *
array_front(Array *arr);

void *
array_back(Array *arr);

inline usize
array_count(Array *arr);

inline usize
array_capacity(Array *arr);

Iterator
array_begin(Array *arr);

Iterator
array_end(Array *arr);

Iterator
array_iterator(Array *arr, usize index);

usize
array_iterator_index(Array *arr, Iterator *iterator);

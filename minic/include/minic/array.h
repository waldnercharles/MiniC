#pragma once

#include "minic/allocator.h"
#include "minic/int.h"
#include "minic/iterator.h"

template <typename T>
struct Array
{
    T &operator[](ssize index);
    const T &operator[](ssize index) const;

    T *data;
    usize count;
    usize capacity;

    Allocator *allocator;
};

template <typename T>
void array_init(Array<T> *arr, Allocator *allocator);

template <typename T>
void array_init(Array<T> *arr, Allocator *allocator, usize capacity);

template <typename T>
void array_free(Array<T> *arr);

template <typename T>
void array_reserve(Array<T> *arr, usize capacity);

template <typename T>
void array_reserve_pow2(Array<T> *arr, usize capacity);

template <typename T>
T *array_begin(Array<T> *arr);

template <typename T>
T *array_end(Array<T> *arr);

template <typename T>
void array_push_back(Array<T> *arr, const T value);

template <typename T>
void array_push_back(Array<T> *arr, const T *range_begin, const T *range_end);

template <typename T>
void array_push_front(Array<T> *arr, const T value);

template <typename T>
void array_push_front(Array<T> *arr, const T *range_begin, const T *range_end);

template <typename T>
void array_pop_back(Array<T> *arr);

template <typename T>
void array_insert(Array<T> *arr, usize index, const T value);

template <typename T>
void array_insert(Array<T> *arr, usize index, usize count, const T value);

template <typename T>
void array_insert(Array<T> *arr,
                  usize index,
                  const T *range_begin,
                  const T *range_end);

template <typename T>
void array_insert(Array<T> *arr, T *insert_pos, const T value);

template <typename T>
void array_insert(Array<T> *arr, T *insert_pos, usize count, const T value);

template <typename T>
void array_insert(Array<T> *arr,
                  T *insert_pos,
                  const T *range_begin,
                  const T *range_end);

template <typename T>
T *array_remove(Array<T> *arr, usize index);

template <typename T>
T *array_remove(Array<T> *arr, T *remove_pos);

template <typename T>
T *array_remove(Array<T> *arr, T *range_begin, T *range_end);

template <typename T>
void array_clear(Array<T> *arr);

template <typename T>
bool array_is_empty(Array<T> *arr);

template <typename T>
T array_at(Array<T> *arr, usize index);

template <typename T>
T array_front(Array<T> *arr);

template <typename T>
T array_back(Array<T> *arr);

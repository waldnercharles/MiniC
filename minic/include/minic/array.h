#pragma once

#include "minic/allocator.h"
#include "minic/int.h"
#include "minic/iterator.h"

template <typename T>
struct Array
{
public:
    Array(Allocator *allocator);
    Array(Allocator *allocator, usize capacity);
    ~Array();

    void reserve(usize capacity);
    void reserve_pow2(usize capacity);

    T *begin();
    T *end();

    const T *begin() const;
    const T *end() const;

    void push_back(const T value);
    void push_back(const T *range_begin, const T *range_end);

    void push_front(const T value);
    void push_front(const T *range_begin, const T *range_end);

    void pop_back();

    void insert(usize index, const T value);
    void insert(usize index, const T *range_begin, const T *range_end);
    void insert(T *insert_pos, const T value);
    void insert(T *insert_pos, const T *range_begin, const T *range_end);

    T *remove(usize index);
    T *remove(T *remove_pos);
    T *remove(T *range_begin, T *range_end);

    void clear();

    // T at(usize index);
    const T at(usize index) const;

    // T front();
    const T front() const;

    // T back();
    const T back() const;

    T &operator[](usize index);
    const T &operator[](usize index) const;

private:
    T *data;
    usize count;
    usize capacity;

    Allocator *allocator;
};

// template <typename T>
// void array_init(Array<T> *arr, Allocator *allocator);
//
// template <typename T>
// void array_init(Array<T> *arr, Allocator *allocator, usize capacity);
//
// template <typename T>
// void array_reserve(Array<T> *arr, usize capacity);
//
// template <typename T>
// void array_reserve_pow2(Array<T> *arr, usize capacity);
//
// template <typename T>
// void array_free(Array<T> *arr);
//
// template <typename T>
// T *array_begin(Array<T> *arr);
//
// template <typename T>
// const T *array_begin(Array<T> *arr);
//
// template <typename T>
// T *array_end(Array<T> *arr);
//
// template <typename T>
// const T *array_end(Array<T> *arr);
//
// template <typename T>
// bool array_is_empty(Array<T> *arr);
//
// template <typename T>
// void array_push_back(Array<T> *arr, const T value);
//
// template <typename T>
// void array_push_back(Array<T> *arr, Array<T> *other);
//
// template <typename T>
// void array_push_front(Array<T> *arr, const T value);
//
// template <typename T>
// void array_push_front(Array<T> *arr, Array<T> *other);
//
// template <typename T>
// void array_insert(Array<T> *arr, usize index, const T value);
//
// template <typename T>
// void array_insert(Array<T> *arr, usize index, Array<T> *other);
//
// template <typename T>
// T array_pop_back(Array<T> *arr);
//
// template <typename T>
// T array_pop_front(Array<T> *arr);
//
// template <typename T>
// void array_remove(Array<T> *arr, usize index);

/*template <typename T>
struct Array
{
    explicit Array(Allocator &allocator);
    explicit Array(const Array<T> &other);
    explicit Array(Array<T> &&other);

    ~Array();

    void reserve(usize capacity);
    void reserve_pow2(usize capacity);

    T *begin();
    const T *begin() const;

    T *end();
    const T *end() const;

    bool is_empty() const;

    usize get_count() const;
    usize get_capacity() const;

    T &operator[](usize index);
    T &at(usize index);
    T &front();
    T &back();

    const T &operator[](usize index) const;
    const T &at(usize index) const;
    const T &front() const;
    const T &back() const;

    void push_back(T &&value);
    void push_back(const T &value);

    void push_front(T &&value);
    void push_front(const T &value);

    void insert(usize index, const T &value);
    void insert(usize index, T &&value);

private:
    T *data = NULL;
    usize count = 0;
    usize capacity = 0;

    Allocator &allocator;
}*/

// void array_push_front(Array *Arr, void *value);
//
// void array_insert(Array *arr, usize index, void *value);
//
// void array_set(Array *arr, usize index, void *value);
//
// void array_pop_back(Array *arr);
//
// void array_pop_front(Array *arr);
//
// void array_iterator_remove(Array *arr, Iterator *iterator);
//
// void array_remove(Array *arr, usize index);
//
// void array_clear(Array *arr);
//
// void *array_get(Array *arr, usize index);
//
// void *array_front(Array *arr);
//
// void *array_back(Array *arr);
//
// inline usize array_count(Array *arr);
//
// inline usize array_capacity(Array *arr);
//
// Iterator array_begin(Array *arr);
//
// Iterator array_end(Array *arr);
//
// Iterator array_iterator(Array *arr, usize index);
//
// usize array_iterator_index(Array *arr, Iterator *iterator);

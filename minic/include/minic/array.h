#pragma once

#include "minic/allocator.h"
#include "minic/int.h"
#include "minic/iterator.h"

template <typename T>
struct Array
{
public:
    explicit Array(Allocator *allocator);
    Array(Allocator *allocator, usize capacity);
    ~Array();

    usize get_count() const;
    usize get_capacity() const;

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
    void insert(usize index, usize count, const T value);
    void insert(usize index, const T *range_begin, const T *range_end);

    void insert(T *insert_pos, const T value);
    void insert(T *insert_pos, usize count, const T value);
    void insert(T *insert_pos, const T *range_begin, const T *range_end);

    T *remove(usize index);
    T *remove(T *remove_pos);
    T *remove(T *range_begin, T *range_end);

    void clear();

    bool is_empty() const;

    T at(usize index);
    T front();
    T back();

    const T at(usize index) const;
    const T front() const;
    const T back() const;

    T &operator[](ssize index);
    const T &operator[](ssize index) const;

private:
    T *data;
    usize count;
    usize capacity;

    Allocator *allocator;

    Array<T> operator=(const Array<T> &copy) = delete;
    Array(const Array<T> &copy) = delete;
};

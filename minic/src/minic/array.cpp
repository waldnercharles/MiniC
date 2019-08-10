#include "minic/array.h"
#include "minic/assert.h"
#include "minic/math.h"
#include "minic/memory.h"
#include "minic/log.h"

template <typename T>
inline Array<T>::Array(Allocator *allocator)
{
    assert(allocator != NULL);

    this->data = NULL;
    this->count = 0;
    this->capacity = 0;
    this->allocator = allocator;
}

template <typename T>
Array<T>::Array(Allocator *allocator, usize capacity)
{
    assert(allocator != NULL);

    this->data = NULL;
    this->count = 0;
    this->capacity = 0;
    this->allocator = allocator;

    this->reserve(capacity);
}

template <typename T>
Array<T>::~Array()
{
    memory_free(this->allocator, this->data);
}

template <typename T>
usize Array<T>::get_count() const
{
    return this->count;
}

template <typename T>
usize Array<T>::get_capacity() const
{
    return this->capacity;
}

template <typename T>
void Array<T>::reserve(usize capacity)
{
    if (capacity > this->capacity)
    {
        T *new_data = memory_realloc(this->allocator, this->data, capacity);

        assert(new_data != NULL);

        this->data = new_data;
        this->capacity = capacity;
    }
}

template <typename T>
void Array<T>::reserve_pow2(usize capacity)
{
    this->reserve(next_pow2(capacity));
}

template <typename T>
T *Array<T>::begin()
{
    assert(this->data != NULL);
    return this->data;
}

template <typename T>
T *Array<T>::end()
{
    assert(this->data != NULL);
    return this->data + this->count;
}

template <typename T>
const T *Array<T>::begin() const
{
    assert(this->data != NULL);
    return this->data;
}

template <typename T>
const T *Array<T>::end() const
{
    assert(this->data != NULL);
    return this->data + this->count;
}

template <typename T>
void Array<T>::push_back(const T value)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->end(), &value, &value + 1);
    /*if (this->count >= this->capacity)
    {
        this->reserve_pow2(this->count + 1);
    }

    assert(this->data != NULL);
    this->data[this->count] = value;

    ++this->count;*/
}

template <typename T>
void Array<T>::push_back(const T *range_begin, const T *range_end)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->end(), range_begin, range_end);
    /*assert(begin != NULL);
    assert(begin >= this->begin());

    assert(end != NULL);
    assert(end <= this->end());

    assert(end > begin);

    if ((this->count + (end - begin)) >= this->capacity)
    {
        this->reserve_pow2(this->count + (end - begin) + 1);
    }

    assert(this->data != NULL);
    mem_copy(this->end(), begin, end - begin);

    this->count += end - begin;*/
}

template <typename T>
void Array<T>::push_front(const T value)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->begin(), value);
}

template <typename T>
void Array<T>::push_front(const T *range_begin, const T *range_end)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->begin(), range_begin, range_end);
}

template <typename T>
void Array<T>::pop_back()
{
    assert(this->count > 0);
    --this->count;
}

template <typename T>
void Array<T>::insert(usize index, const T value)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->begin() + index, &value, &value + 1);
}

template <typename T>
void Array<T>::insert(usize index, usize count, const T value)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->begin() + index, count, value);
}

template <typename T>
void Array<T>::insert(usize index, const T *range_begin, const T *range_end)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(this->begin() + index, range_begin, value);
}

template <typename T>
void Array<T>::insert(T *insert_pos, const T value)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    this->insert(insert_pos, &value, &value + 1);
}

template <typename T>
void Array<T>::insert(T *insert_pos, usize count, const T value)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    assert(insert_pos != NULL);
    assert(insert_pos >= this->data);

    const usize index = cast(usize, (insert_pos - this->begin()));
    assert(index <= this->count);

    const usize new_count = this->count + count;
    if (new_count > this->capacity)
    {
        this->reserve_pow2(new_count + 1);
        insert_pos = this->begin() + index;
    }

    if (insert_pos != this->end())
    {
        memory_copy(insert_pos, this->end(), insert_pos + count);
    }

    memory_set(insert_pos, count, value);

    this->count = new_count;
}

template <typename T>
void Array<T>::insert(T *insert_pos, const T *range_begin, const T *range_end)
{
    if (this->data == NULL)
    {
        this->reserve(32u);
    }

    assert(insert_pos != NULL);
    assert(insert_pos >= this->data);

    const usize index = cast(usize, (insert_pos - this->begin()));
    assert(index <= this->count);

    assert(range_begin != NULL);
    assert(range_end != NULL);

    const ssize range_count = (range_end - range_begin);
    assert(range_count > 0);

    const usize new_count = this->count + cast(usize, range_count);

    if (new_count >= this->capacity)
    {
        this->reserve_pow2(new_count + 1);
        insert_pos = this->begin() + index;
    }

    if (insert_pos != this->end())
    {
        memory_copy(insert_pos, this->end(), insert_pos + range_count);
    }

    memory_copy(range_begin, range_end, insert_pos);

    this->count = new_count;
}

template <typename T>
T *Array<T>::remove(usize index)
{
    return this->remove(this->begin() + index);
}

template <typename T>
T *Array<T>::remove(T *remove_pos)
{
    return this->remove(remove_pos, remove_pos + 1);
}

template <typename T>
T *Array<T>::remove(T *range_begin, T *range_end)
{
    assert(range_begin != NULL);
    assert(range_end != NULL);

    const T *begin = this->begin();
    const T *end = this->end();

    assert(range_begin >= begin);
    assert(range_end <= end);

    const usize range_count = range_end - range_begin;
    assert(range_count > 0);

    if (range_begin == this->begin() && range_end == this->end())
    {
        this->clear();
    }
    else
    {
        memory_copy(range_end, end, range_begin);
        this->count -= range_count;
    }

    return range_begin;
}

template <typename T>
void Array<T>::clear()
{
    this->count = 0;
}

template <typename T>
bool Array<T>::is_empty() const
{
    return this->count == 0;
}

template <typename T>
T Array<T>::at(usize index)
{
    assert(this->data != NULL);
    assert(this->count > index);

    return this->data[index];
}

template <typename T>
T Array<T>::front()
{
    return this->at(0);
}

template <typename T>
T Array<T>::back()
{
    assert(this->count > 0);

    return this->at(this->count - 1);
}

template <typename T>
const T Array<T>::at(usize index) const
{
    assert(this->data != NULL);
    assert(this->count > index);

    return this->data[index];
}

template <typename T>
const T Array<T>::front() const
{
    return this->at(0);
}

template <typename T>
const T Array<T>::back() const
{
    assert(this->count > 0);

    return this->at(this->count - 1);
}

// template <typename T>
// T &Array<T>::operator[](usize index)
//{
//    assert(this->data != NULL);
//    assert(this->count > index);
//
//    return this->data[index];
//}
//
// template <typename T>
// const T &Array<T>::operator[](usize index) const
//{
//    assert(this->data != NULL);
//    assert(this->count > index);
//
//    return this->data[index];
//}

template <typename T>
T &Array<T>::operator[](ssize index)
{
    assert(this->data != NULL);

    assert(index >= 0);
    assert(this->count > cast(usize, index));

    return this->data[index];
}
template <typename T>
const T &Array<T>::operator[](ssize index) const
{
    assert(this->data != NULL);

    assert(index >= 0);
    assert(this->count > cast(usize, index));

    return this->data[index];
}
//
// template <typename T>
// bool array_is_empty(Array<T> *arr)
//{
//    return arr->count == 0;
//}
//
// template <typename T>
// void array_insert(Array<T> *arr, usize index, const T value)
//{
//    assert(arr != NULL);
//    assert(index <= arr->count);
//    if (arr->count >= arr->capacity)
//    {
//        array_reserve_pow2(arr, arr->capacity + 1);
//    }
//
//    T *offset = arr->data + index;
//    mem_move(offset + 1, offset, sizeof(T) * (arr->count - index));
//
//    arr->data[index] = value;
//    ++arr->count;
//}
//
// template <typename T>
// void array_insert(Array<T> *arr, usize index, Array<T> *other)
//{
//    assert(arr != NULL);
//    assert(other != NULL);
//    assert(other->count > 0);
//    assert(index <= arr->count);
//
//    if (arr->count + other->count >= arr->capacity)
//    {
//        array_reserve_pow2(arr, arr->count + other->count + 1);
//    }
//
//    T *offset = arr->data + index;
//
//    mem_move(offset + other->count, offset, sizeof(T) + (arr->count - index));
//    mem_copy(arr->data + index, other->data, sizeof(T) * other->count);
//    arr->count += other->count;
//}
//
// template <typename T>
// T array_pop_back(Array<T> *arr)
//{
//    assert(arr != NULL);
//    assert(arr->count > 0);
//
//    arr->count--;
//
//    return arr->data[count + 1];
//}
//
// template <typename T>
// T array_pop_front(Array<T> *arr)
//{
//    return array_remove(arr, 0);
//}
//
// template <typename T>
// void array_remove(Array<T> *arr, usize index)
//{
//    assert(arr != NULL);
//    assert(index < arr->count);
//
//    u8 *offset = cast(u8 *, arr->data + index);
//    mem_move(offset, offset + sizeof(T), sizeof(T) * (arr->count - index -
//    1));
//
//    arr->count--;
//}
//
// template <typename T>
// void array_clear(Array<T> *arr)
//{
//    arr->count = 0;
//}

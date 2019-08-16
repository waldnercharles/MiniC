#include "minic/array.h"
#include "minic/assert.h"
#include "minic/math.h"
#include "minic/memory.h"
#include "minic/log.h"

template <typename T>
void array_init(Array<T> *arr, Allocator *allocator)
{
    assert(arr != NULL);
    assert(allocator != NULL);

    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
    arr->allocator = allocator;
}

template <typename T>
void array_init(Array<T> *arr, Allocator *allocator, usize capacity)
{
    assert(arr != NULL);
    assert(allocator != NULL);

    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
    arr->allocator = allocator;

    array_reserve(arr, capacity);
}

template <typename T>
void array_free(Array<T> *arr)
{
    assert(arr != NULL);
    memory_free(arr->allocator, arr->data);
}

template <typename T>
void array_reserve(Array<T> *arr, usize capacity)
{
    if (capacity > arr->capacity)
    {
        T *new_data = memory_realloc(arr->allocator, arr->data, capacity);

        assert(new_data != NULL);

        arr->data = new_data;
        arr->capacity = capacity;
    }
}

template <typename T>
void array_reserve_pow2(Array<T> *arr, usize capacity)
{
    array_reserve(arr, next_pow2(capacity));
}

template <typename T>
T *array_begin(Array<T> *arr)
{
    assert(arr->data != NULL);
    return arr->data;
}

template <typename T>
T *array_end(Array<T> *arr)
{
    assert(arr->data != NULL);
    return arr->data + arr->count;
}

template <typename T>
void array_push_back(Array<T> *arr, const T value)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    array_insert(arr, array_end(arr), &value, &value + 1);
    /*if (arr->count >= arr->capacity)
    {
        arr->reserve_pow2(arr->count + 1);
    }

    assert(arr->data != NULL);
    arr->data[arr->count] = value;

    ++arr->count;*/
}

template <typename T>
void array_push_back(Array<T> *arr, const T *range_begin, const T *range_end)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    array_insert(arr, array_end(arr), range_begin, range_end);
    /*assert(begin != NULL);
    assert(begin >= arr->begin());

    assert(end != NULL);
    assert(end <= arr->end());

    assert(end > begin);

    if ((arr->count + (end - begin)) >= arr->capacity)
    {
        arr->reserve_pow2(arr->count + (end - begin) + 1);
    }

    assert(arr->data != NULL);
    mem_copy(arr->end(), begin, end - begin);

    arr->count += end - begin;*/
}

template <typename T>
void array_push_front(Array<T> *arr, const T value)
{
    if (arr->data == NULL)
    {
        arr->reserve(32u);
    }

    arr->insert(arr->begin(), value);
}

template <typename T>
void array_push_front(Array<T> *arr, const T *range_begin, const T *range_end)
{
    if (arr->data == NULL)
    {
        arr->reserve(32u);
    }

    arr->insert(arr->begin(), range_begin, range_end);
}

template <typename T>
void array_pop_back(Array<T> *arr)
{
    assert(arr->count > 0);
    --arr->count;
}

template <typename T>
void array_insert(Array<T> *arr, usize index, const T value)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    array_insert(arr, array_begin(arr) + index, &value, &value + 1);
}

template <typename T>
void array_insert(Array<T> *arr, usize index, usize count, const T value)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    array_insert(arr, array_begin(arr) + index, count, value);
}

template <typename T>
void array_insert(Array<T> *arr,
                  usize index,
                  const T *range_begin,
                  const T *range_end)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    array_insert(arr, array_begin(arr) + index, range_begin, value);
}

template <typename T>
void array_insert(Array<T> *arr, T *insert_pos, const T value)
{
    if (arr->data == NULL)
    {
        arr->reserve(32u);
    }

    array_insert(arr, insert_pos, &value, &value + 1);
}

template <typename T>
void array_insert(Array<T> *arr, T *insert_pos, usize count, const T value)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    assert(insert_pos != NULL);
    assert(insert_pos >= arr->data);

    const usize index = cast(usize, (insert_pos - array_begin(arr)));
    assert(index <= arr->count);

    const usize new_count = arr->count + count;
    if (new_count > arr->capacity)
    {
        array_reserve_pow2(arr, new_count + 1);
        insert_pos = array_begin(arr) + index;
    }

    if (insert_pos != array_end(arr))
    {
        memory_copy(insert_pos, array_end(arr), insert_pos + count);
    }

    memory_set(insert_pos, count, value);

    arr->count = new_count;
}

template <typename T>
void array_insert(Array<T> *arr,
                  T *insert_pos,
                  const T *range_begin,
                  const T *range_end)
{
    if (arr->data == NULL)
    {
        array_reserve(arr, 32u);
    }

    assert(insert_pos != NULL);
    assert(insert_pos >= arr->data);

    const usize index = cast(usize, (insert_pos - array_begin(arr)));
    assert(index <= arr->count);

    assert(range_begin != NULL);
    assert(range_end != NULL);

    const ssize range_count = (range_end - range_begin);
    assert(range_count > 0);

    const usize new_count = arr->count + cast(usize, range_count);

    if (new_count >= arr->capacity)
    {
        array_reserve_pow2(arr, new_count + 1);
        insert_pos = array_begin(arr) + index;
    }

    if (insert_pos != array_end(arr))
    {
        memory_copy(insert_pos, array_end(arr), insert_pos + range_count);
    }

    memory_copy(range_begin, range_end, insert_pos);

    arr->count = new_count;
}

template <typename T>
T *array_remove(Array<T> *arr, usize index)
{
    return array_remove(array_begin(arr) + index);
}

template <typename T>
T *array_remove(Array<T> *arr, T *remove_pos)
{
    return array_remove(remove_pos, remove_pos + 1);
}

template <typename T>
T *array_remove(Array<T> *arr, T *range_begin, T *range_end)
{
    assert(range_begin != NULL);
    assert(range_end != NULL);

    const T *begin = array_begin(arr);
    const T *end = array_end(arr);

    assert(range_begin >= begin);
    assert(range_end <= end);

    const usize range_count = range_end - range_begin;
    assert(range_count > 0);

    if (range_begin == array_begin(arr) && range_end == array_end(arr))
    {
        array_clear();
    }
    else
    {
        memory_copy(range_end, end, range_begin);
        arr->count -= range_count;
    }

    return range_begin;
}

template <typename T>
void array_clear(Array<T> *arr)
{
    arr->count = 0;
}

template <typename T>
bool array_is_empty(Array<T> *arr)
{
    return arr->count == 0;
}

template <typename T>
T array_at(Array<T> *arr, usize index)
{
    assert(arr->data != NULL);
    assert(arr->count > index);

    return arr->data[index];
}

template <typename T>
T array_front(Array<T> *arr)
{
    return array_at(arr, 0);
}

template <typename T>
T array_back(Array<T> *arr)
{
    assert(arr->count > 0);

    return array_at(arr, arr->count - 1);
}

// template <typename T>
// T &array_operator[](usize index)
//{
//    assert(arr->data != NULL);
//    assert(arr->count > index);
//
//    return arr->data[index];
//}
//
// template <typename T>
// const T &array_operator[](usize index) const
//{
//    assert(arr->data != NULL);
//    assert(arr->count > index);
//
//    return arr->data[index];
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

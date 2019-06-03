#include "minic/array.h"
#include "minic/assert.h"
#include "minic/math.h"
#include "minic/memory.h"

overload void
array_init(Array *arr, Allocator *allocator, usize element_size, usize capacity)
{
    assert(arr != NULL);

    arr->allocator = allocator;
    arr->length = 0;
    arr->capacity = 0;
    arr->element_size = element_size;
    arr->data = NULL;

    array_reserve(arr, capacity);
}

overload void
array_init(Array *arr, Allocator *allocator, usize element_size)
{
    array_init(arr, allocator, element_size, ARRAY_DEFAULT_CAPACITY);
}

void
array_free(Array *arr)
{
    assert(arr != NULL);

    allocator_free(arr->allocator, arr->data);
    mem_set(arr, 0, sizeof(*arr));
}

void
array_copy(Array *dst, Array *src)
{
    assert(src != NULL);
    assert(dst != NULL);
    assert(src->data != NULL);

    if (dst->data != NULL)
    {
        array_free(dst);
    }

    array_init(dst, src->allocator, src->element_size, src->capacity);
    mem_copy(dst->data, src->data, src->length * src->element_size);
}

void
array_move(Array *dst, Array *src)
{
    assert(src != NULL);
    assert(dst != NULL);
    assert(src->data != NULL);

    if (dst->data != NULL)
    {
        array_free(dst);
    }

    *dst = *src;
    mem_set(src, 0, sizeof(*src));
}

bool
array_reserve(Array *arr, usize capacity)
{
    assert(arr != NULL);

    if (capacity > arr->capacity)
    {
        void *new_data = allocator_realloc(arr->allocator,
                                           arr->data,
                                           capacity * arr->element_size);

        if (new_data == NULL)
        {
            return false;
        }

        arr->data = new_data;
        arr->capacity = capacity;
    }

    return true;
}

bool
array_reserve_pow2(Array *arr, usize capacity)
{
    return array_reserve(arr, next_pow2(capacity));
}

void
array_push_back(Array *arr, void *value)
{
    assert(arr != NULL);

    if (arr->length == arr->capacity)
        array_reserve_pow2(arr, arr->capacity + 1);

    void *offset = _array_offset(arr, arr->length);
    mem_copy(offset, value, arr->element_size);

    arr->length++;
}

void
array_push_front(Array *arr, void *value)
{
    array_insert(arr, 0, value);
}

void
array_insert(Array *arr, usize index, void *value)
{
    assert(arr != NULL);
    assert(index <= arr->length);

    if (arr->length == arr->capacity)
        array_reserve_pow2(arr, arr->capacity + 1);

    u8 *offset = _array_offset(arr, index);
    mem_move(offset + arr->element_size,
             offset,
             arr->element_size * (arr->length - index));

    mem_copy(offset, value, arr->element_size);
    arr->length++;
}

void
array_set(Array *arr, usize index, void *value)
{
    assert(arr != NULL);
    assert(index < arr->length);

    u8 *offset = _array_offset(arr, index);
    mem_copy(offset, value, arr->element_size);
}

void
array_pop_back(Array *arr)
{
    assert(arr != NULL);
    assert(arr->length > 0);

    arr->length--;
}

void
array_pop_front(Array *arr)
{
    array_remove(arr, cast(usize, 0));
}

overload void
array_remove(Array *arr, usize index)
{
    assert(arr != NULL);
    assert(index < arr->length);

    u8 *offset = _array_offset(arr, index);
    mem_move(offset,
             offset + arr->element_size,
             arr->element_size * (arr->length - index - 1));

    arr->length--;
}

overload void
array_remove(Array *arr, Iterator *iterator)
{
    assert(arr != NULL);
    assert(iterator != NULL);
    assert(arr->element_size == iterator->element_size);

    usize index = array_iterator_index(arr, iterator);
    array_remove(arr, index);

    *iterator = array_iterator(arr, index);
}

void
array_clear(Array *arr)
{
    arr->length = 0;
}

void *
array_get(Array *arr, usize index)
{
    assert(arr != NULL);
    assert(index < arr->length);
    return _array_offset(arr, index);
}

void *
array_front(Array *arr)
{
    return array_get(arr, 0);
}

void *
array_back(Array *arr)
{
    return array_get(arr, arr->length - 1);
}

Iterator
array_begin(Array *arr)
{
    return array_iterator(arr, 0);
}

Iterator
array_end(Array *arr)
{
    return array_iterator(arr, arr->length);
}

Iterator
array_iterator(Array *arr, usize index)
{
    assert(arr != NULL);
    assert(index <= arr->length);

    Iterator iterator;
    iterator.data = _array_offset(arr, index);
    iterator.element_size = arr->element_size;

    return iterator;
}

usize
array_iterator_index(Array *arr, Iterator *iterator)
{
    assert(arr != NULL);
    assert(iterator != NULL);
    assert(iterator->data != NULL);
    assert(arr->data != NULL);

    assert(iterator->data >= arr->data);
    assert(arr->element_size == iterator->element_size);
    assert(arr->element_size > 0);

    return cast(usize, (cast(u8 *, iterator->data) - cast(u8 *, arr->data))) /
           arr->element_size;
}

void *
_array_offset(Array *arr, usize index)
{
    assert(arr != NULL);
    assert(arr->data != NULL);
    assert(arr->element_size > 0);
    return cast(u8 *, arr->data) + (index * arr->element_size);
}

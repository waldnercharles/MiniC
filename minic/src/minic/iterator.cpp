#include "minic/assert.h"
#include "minic/iterator.h"

struct Iterator
{
    usize element_size;
    void *arr;
};

inline void
iterator_init(Iterator *iterator, void *arr, usize element_size)
{
    assert(iterator != NULL);
    assert(arr != NULL);
    assert(element_size != 0);

    iterator->arr = arr;
    iterator->element_size = element_size;
}

inline void *
iterator_current(Iterator *iterator)
{
    return iterator->arr;
}

inline void
iterator_advance(Iterator *iterator, ssize offset)
{
    assert(iterator != NULL);
    assert(iterator->arr != NULL);
    assert(iterator->element_size > 0);
    assert(offset != 0);

    iterator->arr = cast(u8 *, iterator->arr) +
                    cast(ssize, iterator->element_size) * offset;
}

inline void *
iterator_next(Iterator *iterator)
{
    assert(iterator != NULL);
    assert(iterator->arr != NULL);

    void *current = iterator->arr;
    iterator_advance(iterator, 1);

    return current;
}

inline void *
iterator_previous(Iterator *iterator)
{
    assert(iterator != NULL);
    assert(iterator->arr != NULL);

    void *current = iterator->arr;
    iterator_advance(iterator, -1);

    return current;
}

inline bool
iterator_equals(Iterator *lhs, Iterator *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(lhs->element_size == rhs->element_size);

    return lhs->arr == rhs->arr;
}

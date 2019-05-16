#include "minic/assert.h"
#include "minic/iterator.h"

void *
iterator_get(Iterator *iterator)
{
    return iterator->data;
}

void
iterator_advance(Iterator *iterator, s32 offset)
{
    assert(iterator != NULL);
    assert(iterator->data != NULL);
    assert(iterator->element_size > 0);
    assert(offset != 0);

    iterator->data += (iterator->element_size * offset);
}

void *
iterator_next(Iterator *iterator)
{
    assert(iterator != NULL);

    void *current = iterator->data;
    iterator_advance(iterator, 1);

    return current;
}

void *
iterator_previous(Iterator *iterator)
{
    assert(iterator != NULL);

    void *current = iterator->data;
    iterator_advance(iterator, -1);

    return current;
}

bool
iterator_equals(Iterator *lhs, Iterator *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    assert(lhs->element_size == rhs->element_size);
    return lhs->data == rhs->data;
}

bool iterator_

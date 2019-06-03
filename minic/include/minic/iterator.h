#ifndef ITERATOR_H
#define ITERATOR_H

#include "minic/int.h"

typedef struct
{
    usize element_size;
    void *data;
} Iterator;

void *
iterator_get(Iterator *iterator);

void
iterator_advance(Iterator *iterator, usize offset);

void *
iterator_next(Iterator *iterator);

void *
iterator_previous(Iterator *iterator);

#endif

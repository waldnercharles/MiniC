#ifndef ITERATOR_H
#define ITERATOR_H

#include "minic/int.h"

typedef struct
{
    u32 element_size;
    void *data;
} Iterator;

void *
iterator_get(Iterator *iterator);

void
iterator_advance(Iterator *iterator, s32 offset);

void *
iterator_next(Iterator *iterator);

void *
iterator_previous(Iterator *iterator);

#endif
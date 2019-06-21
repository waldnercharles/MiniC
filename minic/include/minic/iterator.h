#pragma once

#include "minic/int.h"

typedef struct Iterator Iterator;

void
iterator_init(Iterator *iterator, void *arr, usize element_size);

void *
iterator_current(Iterator *iterator);

void
iterator_advance(Iterator *iterator, ssize offset);

void *
iterator_next(Iterator *iterator);

void *
iterator_previous(Iterator *iterator);

bool
iterator_equals(Iterator *lhs, Iterator *rhs);

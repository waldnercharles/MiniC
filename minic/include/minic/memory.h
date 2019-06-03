#ifndef MEMORY_H
#define MEMORY_H
#include "minic/int.h"

void *
mem_copy(void *dest, const void *source, usize length);

void *
mem_move(void *dest, const void *source, usize length);

void *
mem_set(void *dest, u8 value, usize length);

s32
mem_compare(const void *lhs, const void *rhs, usize length);

#endif

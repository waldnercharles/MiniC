#ifndef MEMORY_H
#define MEMORY_H
#include "minic/int.h"

void *mem_copy(void *dest, const void *source, u64 length);
void *mem_move(void *dest, const void *source, u64 length);
void *mem_set(void *dest, u32 value, u64 length);

s32 mem_compare(const void *lhs, const void *rhs, u64 length);

#endif
#pragma once
#include "minic/int.h"

template <typename T, typename TDest>
void memory_copy(const T *src_begin, const T *src_end, TDest *dst_begin);

void *mem_copy(void *dest, const void *source, usize length);

void *mem_move(void *dest, const void *source, usize length);

void *mem_set(void *dest, u8 value, usize length);

s32 mem_compare(const void *lhs, const void *rhs, usize length);

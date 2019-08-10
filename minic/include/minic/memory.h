#pragma once
#include "minic/int.h"

template <typename TSource, typename TDest>
void memory_copy(const TSource *src_begin, const TSource *src_end, TDest *dst);

template <typename TDest>
void memory_set(TDest *dst, usize count, const TDest value);

void *mem_copy(void *dest, const void *source, usize length);

void *mem_move(void *dest, const void *source, usize length);

void *mem_set(void *dest, u8 value, usize length);

s32 mem_compare(const void *lhs, const void *rhs, usize length);

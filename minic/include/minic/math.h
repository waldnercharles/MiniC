#ifndef MATH_H
#define MATH_H

#include "minic/int.h"

overload u32
min(u32 x, u32 y);
overload u64
min(u64 x, u64 y);
overload s32
min(s32 x, s32 y);
overload s64
min(s64 x, s64 y);
overload r32
min(r32 x, r32 y);
overload r64
min(r64 x, r64 y);

overload u32
max(u32 x, u32 y);
overload u64
max(u64 x, u64 y);
overload s32
max(s32 x, s32 y);
overload s64
max(s64 x, s64 y);
overload r32
max(r32 x, r32 y);
overload r64
max(r64 x, r64 y);

bool
r64_isnan(r64 value);
bool
r64_isinf(r64 value);

overload u32
next_pow2(u32 x);
overload u64
next_pow2(u64 x);

#endif

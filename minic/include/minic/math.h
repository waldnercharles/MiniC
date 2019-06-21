#pragma once

#include "minic/int.h"

/* clang-format off */
overload inline u32 min(u32 x, u32 y);
overload inline u64 min(u64 x, u64 y);
overload inline s32 min(s32 x, s32 y);
overload inline s64 min(s64 x, s64 y);
overload inline r32 min(r32 x, r32 y);
overload inline r64 min(r64 x, r64 y);

overload inline u32 max(u32 x, u32 y);
overload inline u64 max(u64 x, u64 y);
overload inline s32 max(s32 x, s32 y);
overload inline s64 max(s64 x, s64 y);
overload inline r32 max(r32 x, r32 y);
overload inline r64 max(r64 x, r64 y);

bool r64_isnan(r64 value);
bool r64_isinf(r64 value);

overload u32 next_pow2(u32 x);
overload u64 next_pow2(u64 x);

overload inline u32 rotate_left(u32 x, s32 k);
overload inline u64 rotate_left(u64 x, s64 k);

overload inline u32 rotate_right(u32 x, s32 k);
overload inline u64 rotate_right(u64 x, s64 k);
/* clang-format on */

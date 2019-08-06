#include "minic/math.h"

/* clang-format off */
overload inline u8 min(u8 x, u8 y) { return x < y ? x : y; }
overload inline s8 min(s8 x, s8 y) { return x < y ? x : y; }

overload inline u16 min(u16 x, u16 y) { return x < y ? x : y; }
overload inline s16 min(s16 x, s16 y) { return x < y ? x : y; }

overload inline u32 min(u32 x, u32 y) { return x < y ? x : y; }
overload inline s32 min(s32 x, s32 y) { return x < y ? x : y; }

overload inline u64 min(u64 x, u64 y) { return x < y ? x : y; }
overload inline s64 min(s64 x, s64 y) { return x < y ? x : y; }

overload inline r32 min(r32 x, r32 y) { return x < y ? x : y; }
overload inline r64 min(r64 x, r64 y) { return x < y ? x : y; }

overload inline u8 max(u8 x, u8 y) { return x > y ? x : y; }
overload inline s8 max(s8 x, s8 y) { return x > y ? x : y; }

overload inline u16 max(u16 x, u16 y) { return x > y ? x : y; }
overload inline s16 max(s16 x, s16 y) { return x > y ? x : y; }

overload inline u32 max(u32 x, u32 y) { return x > y ? x : y; }
overload inline s32 max(s32 x, s32 y) { return x > y ? x : y; }

overload inline u64 max(u64 x, u64 y) { return x > y ? x : y; }
overload inline s64 max(s64 x, s64 y) { return x > y ? x : y; }

overload inline r32 max(r32 x, r32 y) { return x > y ? x : y; }
overload inline r64 max(r64 x, r64 y) { return x > y ? x : y; }
/* clang-format on */

bool
r64_isnan(r64 value)
{
    union
    {
        u64 u64;
        r64 r64;
    } c;

    c.r64 = value;
    return (cast(u32, c.u64 >> 32) & 0x7fffffff) + (cast(u32, c.u64) != 0) >
           0x7ff00000;
}

bool
r64_isinf(r64 value)
{
    union
    {
        u64 u64;
        r64 r64;
    } c;

    c.r64 = value;
    return (cast(u32, c.u64 >> 32) & 0x7fffffff) == 0x7ff00000 &&
           (cast(u32, c.u64) == 0);
}

overload u32
next_pow2(u32 x)
{
    x += (x == 0);
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;

    return x;
}

overload u64
next_pow2(u64 x)
{
    x += (x == 0);
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    x++;

    return x;
}

#define rotate_left_bits(x, k, b) ((x << k) | (x >> (b - k)))
#define rotate_right_bits(x, k, b) ((x >> k) | (x << (b - k)))

overload inline u32
rotate_left(u32 x, s32 k)
{
    return rotate_left_bits(x, k, 32);
}

overload inline u64
rotate_left(u64 x, s64 k)
{
    return rotate_left_bits(x, k, 64);
}

overload inline u32
rotate_right(u32 x, s32 k)
{
    return rotate_right_bits(x, k, 32);
}

overload inline u64
rotate_right(u64 x, s64 k)
{
    return rotate_right_bits(x, k, 64);
}

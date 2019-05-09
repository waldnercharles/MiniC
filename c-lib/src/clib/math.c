#include "clib/math.h"

/* clang-format off */
overload u32 min(u32 x, u32 y) { return x < y ? x : y; }
overload u64 min(u64 x, u64 y) { return x < y ? x : y; }
overload s32 min(s32 x, s32 y) { return x < y ? x : y; }
overload s64 min(s64 x, s64 y) { return x < y ? x : y; }
overload r32 min(r32 x, r32 y) { return x < y ? x : y; }
overload r64 min(r64 x, r64 y) { return x < y ? x : y; }

overload u32 max(u32 x, u32 y) { return x > y ? x : y; }
overload u64 max(u64 x, u64 y) { return x > y ? x : y; }
overload s32 max(s32 x, s32 y) { return x > y ? x : y; }
overload s64 max(s64 x, s64 y) { return x > y ? x : y; }
overload r32 max(r32 x, r32 y) { return x > y ? x : y; }
overload r64 max(r64 x, r64 y) { return x > y ? x : y; }
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

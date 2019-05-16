#include "minic/assert.h"
#include "minic/int.h"
#include "minic/memory.h"

void *
mem_copy(void *dst, const void *src, u64 length)
{
    assert(dst != NULL);
    assert(src != NULL);

    u8 *d = dst;
    const u8 *s = src;
    while (length--)
    {
        *d++ = *s++;
    }

    return dst;
}

void *
mem_move(void *dst, const void *src, u64 length)
{
    assert(dst != NULL);
    assert(src != NULL);

    u8 *d = dst;
    const u8 *s = src;

    if (length > 0)
    {
        if (d < s)
        {
            while (length--)
            {
                *d++ = *s++;
            }
        }
        else
        {
            u8 *last_d = (d + (length - 1));
            const u8 *last_s = (s + (length - 1));
            while (length--)
            {
                *last_d-- = *last_s--;
            }
        }
    }

    return dst;
}

void *
mem_set(void *dst, u32 value, u64 length)
{
    assert(dst != NULL);

    u8 *ptr = dst;
    while (length--)
    {
        *ptr++ = cast(u8, value);
    }

    return dst;
}

s32
mem_compare(const void *lhs, const void *rhs, u64 length)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    const u8 *l = lhs;
    const u8 *r = rhs;

    while (length--)
    {
        if (*l++ != *r++)
        {
            return l[-1] < r[-1] ? -1 : 1;
        }
    }

    return 0;
}
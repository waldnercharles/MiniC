#include "minic/assert.h"
#include "minic/int.h"
#include "minic/log.h"
#include "minic/memory.h"

template <typename TSource, typename TDest>
void memory_copy(const TSource *src_begin, const TSource *src_end, TDest *dst)
{
    assert(sizeof(TSource) == sizeof(TDest));

    assert(src_begin != NULL);
    assert(src_end != NULL);
    assert(dst != NULL);

    assert(src_end > src_begin);

    TSource *dst_begin = reinterpret_cast<TSource *>(dst);
    TSource *dst_end = dst + (src_end - src_begin);

    if (dst_begin < src_begin)
    {
        log_debug("Destination range is before source range. Copying left.");
        while (src_begin != src_end)
        {
            *(dst_begin++) = *(src_begin++);
        }
    }
    else if (dst_end > src_end)
    {
        log_debug("Destination range is after source range. Copying right.");
        while (src_begin != src_end)
        {
            *(--dst_end) = *(--src_end);
        }
    }
    else
    {
        log_error("Destination range is contained by source range.");
    }
}

void *mem_copy(void *dst, const void *src, usize length)
{
    assert(dst != NULL);
    assert(src != NULL);
    // assert(dst + length < src);

    u8 *d = reinterpret_cast<u8 *>(dst);
    const u8 *s = reinterpret_cast<const u8 *>(src);
    while (length--)
    {
        *d++ = *s++;
    }

    return dst;
}

void *mem_move(void *dst, const void *src, usize length)
{
    assert(dst != NULL);
    assert(src != NULL);

    u8 *d = reinterpret_cast<u8 *>(dst);
    const u8 *s = reinterpret_cast<const u8 *>(src);

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

void *mem_set(void *dst, u8 value, usize length)
{
    assert(dst != NULL);

    u8 *ptr = static_cast<u8 *>(dst);
    while (length--)
    {
        *ptr++ = cast(u8, value);
    }

    return dst;
}

s32 mem_compare(const void *lhs, const void *rhs, usize length)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    const u8 *l = static_cast<const u8 *>(lhs);
    const u8 *r = static_cast<const u8 *>(rhs);

    while (length--)
    {
        if (*l++ != *r++)
        {
            return l[-1] < r[-1] ? -1 : 1;
        }
    }

    return 0;
}

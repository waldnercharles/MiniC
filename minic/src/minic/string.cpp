#include "minic/string.h"
#include "minic/int.h"
#include "minic/assert.h"

static inline bool
string_has_null(u32 aligned_addr)
{
    return ((aligned_addr - 0x01010101) & ((~aligned_addr) & 0x80808080)) > 0;
}

static inline bool
string_has_char(u32 aligned_addr, u32 mask)
{
    return string_has_null(aligned_addr ^ mask);
}

usize
string_length(const char *str)
{
    ReadOnlyRawBuffer buf;
    buf.str = str;

    usize n = 0;
    bool unaligned = reinterpret_cast<uptr>(str) & 3;
    for (;;)
    {
        assert(buf.u32 != NULL);

        u32 word = *buf.u32;
        bool has_null = string_has_null(word);

        if (unaligned || has_null)
        {
            do
            {
                assert(buf.str != NULL);
                if (*buf.str == 0)
                {
                    return n;
                }

                ++buf.str;
                ++n;
            } while (reinterpret_cast<uptr>(buf.str) & 3);

            unaligned = false;
        }
        else
        {
            buf.str += 4;
            n += 4;
        }
    }
}

usize
string_copy(char *dst, const char *src, usize max)
{
    assert(dst != NULL);
    assert(src != NULL);

    // TODO: Implement fast copy for aligned strings

    s8 c = 0;
    u32 n = 0;
    do
    {
        assert(n < max);

        c = *src++;
        *dst++ = c;
        ++n;
    } while (c);

    return n;
}

const char *
string_find_char(const char *str, const char i)
{
    ReadOnlyRawBuffer buf;
    buf.str = str;

    bool unaligned = reinterpret_cast<uptr>(str) & 3;

    u32 c = cast(u32, i);
    u32 mask = (c << 24) | (c << 16) | (c << 8) | c;
    for (;;)
    {
        assert(buf.u32 != NULL);

        u32 word = *buf.u32;

        bool has_null = string_has_null(word);
        bool has_char = string_has_char(word, mask);

        if (unaligned || has_null || has_char)
        {
            do
            {
                assert(buf.str != NULL);

                if (*buf.str == i)
                {
                    return buf.str;
                }

                if (*buf.str == 0)
                {
                    return NULL;
                }

                ++buf.str;

            } while (reinterpret_cast<uptr>(buf.str) & 3);

            unaligned = false;
        }
        else
        {
            buf.str += 4;
        }
    }
}

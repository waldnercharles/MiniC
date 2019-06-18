#include "minic/string.h"
#include "minic/int.h"
#include "minic/assert.h"

usize
string_length(char *str)
{
    RawBuffer buf = { .str = str };

    usize n = 0;
    bool unaligned = cast(uptr, str) & 3;
    for (;;)
    {
        assert(buf.u32 != NULL);

        u32 word = *buf.u32;
        u32 has_null = (word - 0x01010101) & ((~word) & 0x80808080);

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
            } while (cast(uptr, buf.str) & 3);

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
string_copy(char *dst, char *src, usize max)
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

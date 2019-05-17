#include "minic/string.h"
#include "minic/int.h"
#include "minic/assert.h"

u32
string_length(const char *str)
{
    u32 n = 0;
    bool unaligned = cast(ptr_t, str) & 3;
    for (;;)
    {
        u32 word = *cast(u32 *, str);
        u32 has_null = (word - 0x01010101) & ((~word) & 0x80808080);

        if (unaligned || has_null)
        {
            do
            {
                if (*str == 0)
                {
                    return n;
                }

                ++str;
                ++n;
            } while ((unaligned = cast(ptr_t, str) & 3));
        }
        else
        {
            str += 4;
            n += 4;
        }
    }
}
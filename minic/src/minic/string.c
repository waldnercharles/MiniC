#include "minic/string.h"
#include "minic/int.h"
#include "minic/assert.h"

u32
string_length(const char *str)
{
    u32 n = 0;
    while (cast(ptr_t, str) & 3)
    {
    null_check:
        if (*str == 0)
        {
            return n;
        }

        ++str;
		++n;
    }

    for (;;)
    {
        u32 word = *cast(u32 *, str);
        u32 has_null = (word - 0x01010101) & ((~word) & 0x80808080);

        if (has_null)
        {
			goto null_check;
		}

		str += 4;
        n += 4;
    }
}
#include <minic/int.h>
#include <minic/assert.h>

u32
string_copy(char *dst, const char *src, s32 start, s32 end)
{
    s32 n = start;

    while (cast(ptr_t, src) & 3)
    {
    null_check:
		assert(n < end);
        if (*src == 0)
        {
            return n;
        }

        dst[n] = *src++;
        ++n;

    }
    for (;;)
    {
        u32 word = *cast(u32 *, src);
        u32 has_null = (word - 0x01010101) & (~word) & 0x80808080;

        if (has_null)
        {
            goto null_check;
		}

		assert(n < end);

    }
}
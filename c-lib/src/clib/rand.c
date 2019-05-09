#include "clib/int.h"

// Mersenne Twister
#define RAND_MT_LENGTH 624u
#define RAND_MT_PERIOD 397u
#define RAND_MT_DIFF (RAND_MT_LENGTH - RAND_MT_PERIOD)

#define RAND_MT_TWIST(buf, i, j)                                               \
    (((buf)[i] & 0x80000000) | ((buf)[j] & 0x7fffffff))
#define RAND_MT_MAGIC(x) (((x)&1) * 0x9908b0df)

struct
{
    u64 buffer[RAND_MT_LENGTH];
    u64 index;
} rand_state = { .index = RAND_MT_LENGTH * sizeof(u64) + 1 };

static u64
rand__lcg(u64 seed)
{
    seed = seed * 2147001325 + 715136305;  // BCPL generator
    // shuffle non-random bits to the middle, and xor to decorrelate with seed
    return 0x31415926 ^ ((seed >> 16) + (seed << 16));
}

void
rand_seed(u64 value)
{
    rand_state.buffer[0] = value;
    rand_state.index = RAND_MT_LENGTH * sizeof(u64);

    for (u32 i = 1; i < RAND_MT_LENGTH; i++)
    {
        rand_state.buffer[i] = rand__lcg(rand_state.buffer[i - 1]);
    }
}

u64
rand_generate()
{
    u64 *buf = rand_state.buffer;
    u32 idx = cast(u32, rand_state.index);

    u64 tmp;

    if (idx >= (RAND_MT_LENGTH * sizeof(u64)))
    {
        if (idx > (RAND_MT_LENGTH * sizeof(u64)))
        {
            rand_seed(0);
        }

        idx = 0;

        u32 i = 0;
        for (; i < RAND_MT_DIFF; i++)
        {
            tmp = RAND_MT_TWIST(buf, i, i + 1);
            tmp = (tmp >> 1) ^ RAND_MT_MAGIC(tmp);

            buf[i] = buf[i + RAND_MT_PERIOD] ^ tmp;
        }

        for (; i < (RAND_MT_LENGTH - 1); i++)
        {
            tmp = RAND_MT_TWIST(buf, i, i + 1);
            tmp = (tmp >> 1) ^ RAND_MT_MAGIC(tmp);

            buf[i] = buf[i - RAND_MT_DIFF] ^ tmp;
        }

        tmp = RAND_MT_TWIST(buf, RAND_MT_LENGTH - 1, 0);
        tmp = (tmp >> 1) ^ RAND_MT_MAGIC(tmp);

        buf[RAND_MT_LENGTH - 1] = buf[RAND_MT_PERIOD - 1] ^ tmp;
    }

    rand_state.index = idx + sizeof(u64);

    tmp = *cast(u64 *, cast(u8 *, buf) + idx);
    tmp ^= (tmp >> 11);
    tmp ^= (tmp << 7) & 0x9d2c5680;
    tmp ^= (tmp << 15) & 0xEFC60000;
    tmp ^= (tmp >> 18);

    return tmp;
}

r64
rand_generate_r64()
{
    return rand_generate() / (cast(r64, 1 << 16) * (1 << 16));
}

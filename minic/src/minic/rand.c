#include "minic/rand.h"
#include "minic/int.h"

// Mersenne Twister
#define RAND_MT_LENGTH 624u
#define RAND_MT_PERIOD 397u
#define RAND_MT_DIFF (RAND_MT_LENGTH - RAND_MT_PERIOD)

#define RAND_MT_TWIST(buf, i, j)                                               \
    (((buf)[i] & 0x80000000) | ((buf)[j] & 0x7fffffff))

#define RAND_MT_MAGIC(x) (((x)&1) * 0x9908b0df)

// TODO: Move into non-static struct
static struct
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
    RawBuffer buf;

    buf.u64 = rand_state.buffer;
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
            tmp = RAND_MT_TWIST(buf.u64, i, i + 1);
            tmp = (tmp >> 1) ^ RAND_MT_MAGIC(tmp);

            buf.u64[i] = buf.u64[i + RAND_MT_PERIOD] ^ tmp;
        }

        for (; i < (RAND_MT_LENGTH - 1); i++)
        {
            tmp = RAND_MT_TWIST(buf.u64, i, i + 1);
            tmp = (tmp >> 1) ^ RAND_MT_MAGIC(tmp);

            buf.u64[i] = buf.u64[i - RAND_MT_DIFF] ^ tmp;
        }

        tmp = RAND_MT_TWIST(buf.u64, RAND_MT_LENGTH - 1, 0);
        tmp = (tmp >> 1) ^ RAND_MT_MAGIC(tmp);

        buf.u64[RAND_MT_LENGTH - 1] = buf.u64[RAND_MT_PERIOD - 1] ^ tmp;
    }

    rand_state.index = idx + sizeof(u64);

    buf.u8 += idx;

    tmp = *buf.u64;
    tmp ^= (tmp >> 11);
    tmp ^= (tmp << 7) & 0x9d2c5680;
    tmp ^= (tmp << 15) & 0xEFC60000;
    tmp ^= (tmp >> 18);

    return tmp;
}

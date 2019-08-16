#include "minic/math.h"
#include "minic/rand.h"
#include "minic/int.h"

static u64 rand_splitmix64(u64 val)
{
    val += 0x9e3779b97f4a7c15;
    val = (val ^ (val >> 30)) * 0xbf58476d1ce4e5b9;
    val = (val ^ (val >> 27)) * 0x94d049bb133111eb;
    return val ^ (val >> 31);
}

static u64 rand_xoroshiro128plus(u64 s[2])
{
    u64 s0 = s[0];
    u64 s1 = s[1];
    u64 result = s0 + s1;

    s1 ^= s0;
    s[0] = rotate_left(s0, 24ull) ^ s1 ^ (s1 << 16);
    s[1] = rotate_left(s1, 37ull);

    return result;
}

void rand_seed(Random *rand, u64 seed)
{
    rand->s[0] = rand_splitmix64(seed);
    rand->s[1] = rand_splitmix64(rand->s[0]);
}

u64 rand_next_u64(Random *rand)
{
    return rand_xoroshiro128plus(rand->s);
}

#pragma once

#include "minic/int.h"

typedef struct
{
    u64 s[2];
} Random;

void
rand_seed(Random *rand, u64 seed);

u64
rand_next_u64(Random *rand);

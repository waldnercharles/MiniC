#ifndef RAND_H
#define RAND_H

#include "clib/int.h"

void rand_seed(u64 value);

u64 rand_generate();
r64 rand_generate_r64();

#endif
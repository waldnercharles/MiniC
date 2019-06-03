#ifndef RAND_H
#define RAND_H

#include "minic/int.h"

void
rand_seed(u64 value);

u64
rand_generate(void);

r64
rand_generate_r64(void);

#endif

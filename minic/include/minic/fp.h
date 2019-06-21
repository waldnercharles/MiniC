#pragma once

#include "minic/int.h"

struct fp
{
    u64 f;
    s32 e;
};

struct fp
fp_from_r64(r64 r);

struct fp
fp_subtract(struct fp x, struct fp y);

struct fp
fp_multiply(struct fp lhs, struct fp rhs);

struct fp
fp_normalize(struct fp in);

struct fp
fp_normalize_boundary(struct fp in);

void
fp_normalize_boundaries(struct fp in, struct fp *minus, struct fp *plus);

struct fp
fp_get_cached_power(s32 e, s32 *K);

void
grisu2_round(char *buffer,
             usize len,
             u64 delta,
             u64 rest,
             u64 ten_kappa,
             u64 wp_w);

void
grisu2_digit_gen(struct fp W,
                 struct fp Mp,
                 u64 delta,
                 char *buffer,
                 usize *len,
                 s32 *K);

void
grisu2(r64 value, char *buffer, usize *length, s32 *K);

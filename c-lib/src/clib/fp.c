#include "clib/fp.h"

#include "clib/assert.h"
#include "clib/math.h"
#include "clib/int.h"
#include "clib/memory.h"

#define FP_SIGNIFICAND_SIZE 64
#define DP_SIGNIFICAND_SIZE 52
#define DP_EXPONENT_BIAS (0x3FF + DP_SIGNIFICAND_SIZE)
#define DP_MIN_EXPONENT (-DP_EXPONENT_BIAS)
#define DP_EXPONENT_MASK 0x7FF0000000000000
#define DP_SIGNIFICAND_MASK 0x000FFFFFFFFFFFFF
#define DP_HIDDEN_BIT 0x0010000000000000

struct fp
fp_from_r64(r64 r)
{
    union
    {
        r64 r64;
        u64 u64;
    } u;

    u.r64 = r;

    s32 biased_e = (u.u64 & DP_EXPONENT_MASK) >> DP_SIGNIFICAND_SIZE;
    u64 significand = (u.u64 & DP_SIGNIFICAND_MASK);

    struct fp result;
    if (biased_e != 0)
    {
        result.f = significand + DP_HIDDEN_BIT;
        result.e = biased_e - DP_EXPONENT_BIAS;
    }
    else
    {
        result.f = significand;
        result.e = DP_MIN_EXPONENT + 1;
    }

    return result;
}

struct fp
fp_subtract(struct fp x, struct fp y)
{
    assert(x.e == y.e);
    assert(x.f >= y.f);
    struct fp result;

    result.f = x.f - y.f;
    result.e = x.e;

    return result;
}

struct fp
fp_multiply(struct fp lhs, struct fp rhs)
{
    const u64 max_32 = 0xffffffff;

    const u64 a = lhs.f >> 32;
    const u64 b = lhs.f & max_32;
    const u64 c = rhs.f >> 32;
    const u64 d = rhs.f & max_32;

    const u64 ac = a * c;
    const u64 bc = b * c;
    const u64 ad = a * d;
    const u64 bd = b * d;

    u64 tmp = (bd >> 32) + (ad & max_32) + (bc & max_32);
    tmp += 1U << 31;

    struct fp result;
    result.f = ac + (ad >> 32) + (bc >> 32) + (tmp >> 32);
    result.e = lhs.e + rhs.e + 64;

    return result;
}

struct fp
fp_normalize(struct fp in)
{
    struct fp result = in;
    while (!(result.f & DP_HIDDEN_BIT))
    {
        result.f <<= 1;
        result.e--;
    }

    result.f <<= (FP_SIGNIFICAND_SIZE - DP_SIGNIFICAND_SIZE - 1);
    result.e = result.e - (FP_SIGNIFICAND_SIZE - DP_SIGNIFICAND_SIZE - 1);
    return result;
}

struct fp
fp_normalize_boundary(struct fp in)
{
    struct fp result = in;
    while (!(result.f & (DP_HIDDEN_BIT << 1)))
    {
        result.f <<= 1;
        result.e--;
    }

    result.f <<= (FP_SIGNIFICAND_SIZE - DP_SIGNIFICAND_SIZE - 2);
    result.e = result.e - (FP_SIGNIFICAND_SIZE - DP_SIGNIFICAND_SIZE - 2);
    return result;
}

void
fp_normalize_boundaries(struct fp in, struct fp *minus, struct fp *plus)
{
    struct fp pl, mi;

    pl.f = (in.f << 1) + 1;
    pl.e = in.e - 1;
    pl = fp_normalize_boundary(pl);

    if (in.f == DP_HIDDEN_BIT)
    {
        mi.f = (in.f << 2) - 1;
        mi.e = in.e - 2;
    }
    else
    {
        mi.f = (in.f << 1) - 1;
        mi.e = in.e - 1;
    }

    mi.f <<= mi.e - pl.e;
    mi.e = pl.e;

    *plus = pl;
    *minus = mi;
}

struct fp
fp_get_cached_power(s32 e, s32 *K)
{
    static const u64 cached_powers_f[] =
        { 0xfa8fd5a0081c0288, 0xbaaee17fa23ebf76, 0x8b16fb203055ac76,
          0xcf42894a5dce35ea, 0x9a6bb0aa55653b2d, 0xe61acf033d1a45df,
          0xab70fe17c79ac6ca, 0xff77b1fcbebcdc4f, 0xbe5691ef416bd60c,
          0x8dd01fad907ffc3c, 0xd3515c2831559a83, 0x9d71ac8fada6c9b5,
          0xea9c227723ee8bcb, 0xaecc49914078536d, 0x823c12795db6ce57,
          0xc21094364dfb5637, 0x9096ea6f3848984f, 0xd77485cb25823ac7,
          0xa086cfcd97bf97f4, 0xef340a98172aace5, 0xb23867fb2a35b28e,
          0x84c8d4dfd2c63f3b, 0xc5dd44271ad3cdba, 0x936b9fcebb25c996,
          0xdbac6c247d62a584, 0xa3ab66580d5fdaf6, 0xf3e2f893dec3f126,
          0xb5b5ada8aaff80b8, 0x87625f056c7c4a8b, 0xc9bcff6034c13053,
          0x964e858c91ba2655, 0xdff9772470297ebd, 0xa6dfbd9fb8e5b88f,
          0xf8a95fcf88747d94, 0xb94470938fa89bcf, 0x8a08f0f8bf0f156b,
          0xcdb02555653131b6, 0x993fe2c6d07b7fac, 0xe45c10c42a2b3b06,
          0xaa242499697392d3, 0xfd87b5f28300ca0e, 0xbce5086492111aeb,
          0x8cbccc096f5088cc, 0xd1b71758e219652c, 0x9c40000000000000,
          0xe8d4a51000000000, 0xad78ebc5ac620000, 0x813f3978f8940984,
          0xc097ce7bc90715b3, 0x8f7e32ce7bea5c70, 0xd5d238a4abe98068,
          0x9f4f2726179a2245, 0xed63a231d4c4fb27, 0xb0de65388cc8ada8,
          0x83c7088e1aab65db, 0xc45d1df942711d9a, 0x924d692ca61be758,
          0xda01ee641a708dea, 0xa26da3999aef774a, 0xf209787bb47d6b85,
          0xb454e4a179dd1877, 0x865b86925b9bc5c2, 0xc83553c5c8965d3d,
          0x952ab45cfa97a0b3, 0xde469fbd99a05fe3, 0xa59bc234db398c25,
          0xf6c69a72a3989f5c, 0xb7dcbf5354e9bece, 0x88fcf317f22241e2,
          0xcc20ce9bd35c78a5, 0x98165af37b2153df, 0xe2a0b5dc971f303a,
          0xa8d9d1535ce3b396, 0xfb9b7cd9a4a7443c, 0xbb764c4ca7a44410,
          0x8bab8eefb6409c1a, 0xd01fef10a657842c, 0x9b10a4e5e9913129,
          0xe7109bfba19c0c9d, 0xac2820d9623bf429, 0x80444b5e7aa7cf85,
          0xbf21e44003acdd2d, 0x8e679c2f5e44ff8f, 0xd433179d9c8cb841,
          0x9e19db92b4e31ba9, 0xeb96bf6ebadf77d9, 0xaf87023b9bf0ee6b };

    static const s32 cached_powers_e[] =
        { -1220, -1193, -1166, -1140, -1113, -1087, -1060, -1034, -1007, -980,
          -954,  -927,  -901,  -874,  -847,  -821,  -794,  -768,  -741,  -715,
          -688,  -661,  -635,  -608,  -582,  -555,  -529,  -502,  -475,  -449,
          -422,  -396,  -369,  -343,  -316,  -289,  -263,  -236,  -210,  -183,
          -157,  -130,  -103,  -77,   -50,   -24,   +3,    +30,   +56,   +83,
          +109,  +136,  +162,  +189,  +216,  +242,  +269,  +295,  +322,  +348,
          +375,  +402,  +428,  +455,  +481,  +508,  +534,  +561,  +588,  +614,
          +641,  +667,  +694,  +720,  +747,  +774,  +800,  +827,  +853,  +880,
          +907,  +933,  +960,  +986,  +1013, +1039, +1066 };

    r64 dk = (-61 - e) * 0.30102999566398114 + 347;
    s32 k = cast(s32, dk);
    if (k != dk)
    {
        k++;
    }

    u32 idx = (k >> 3) + 1;
    *K = -(-348 + cast(s32, idx << 3));

    assert(idx < sizeof(cached_powers_f) / sizeof(cached_powers_f[0]));

    struct fp result;
    result.f = cached_powers_f[idx];
    result.e = cached_powers_e[idx];

    return result;
}

inline void
grisu2_round(char *buffer,
             s32 len,
             u64 delta,
             u64 rest,
             u64 ten_kappa,
             u64 wp_w)
{
    while ((rest < wp_w) && ((delta - rest) >= ten_kappa) &&
           (((rest + ten_kappa) < wp_w) ||
            ((wp_w - rest) > (rest + ten_kappa - wp_w))))
    {
        buffer[len - 1]--;
        rest += ten_kappa;
    }
}

inline void
grisu2_digit_gen(struct fp W,
                 struct fp Mp,
                 u64 delta,
                 char *buffer,
                 s32 *len,
                 s32 *K)
{
    static const u32 pow_10[] = { 1,      10,      100,      1000,     10000,
                                  100000, 1000000, 10000000, 100000000 };
    struct fp one;
    one.f = 1ULL << -Mp.e;
    one.e = Mp.e;

    struct fp wp_w = fp_subtract(Mp, W);

    u32 p1 = cast(u32, Mp.f >> -one.e);
    u64 p2 = Mp.f & (one.f - 1);

    /* clang-format off */
    s32 kappa;
    if (p1 < 10) kappa = 1;
	else if (p1 < 100) kappa = 2;
	else if (p1 < 1000) kappa = 3;
	else if (p1 < 10000) kappa = 4;
	else if (p1 < 100000) kappa = 5;
	else if (p1 < 1000000) kappa = 6;
	else if (p1 < 10000000) kappa = 7;
	else if (p1 < 100000000) kappa = 8;
	else if (p1 < 1000000000) kappa = 9;
	else kappa = 10;
    /* clang-format on */

    *len = 0;

    while (kappa > 0)
    {
        /* clang-format off */
		u32 d;
		switch (kappa)
		{
			case 10: d = p1 / 1000000000; p1 %= 1000000000; break;
			case  9: d = p1 /  100000000; p1 %=  100000000; break;
			case  8: d = p1 /   10000000; p1 %=   10000000; break;
			case  7: d = p1 /    1000000; p1 %=    1000000; break;
			case  6: d = p1 /     100000; p1 %=     100000; break;
			case  5: d = p1 /      10000; p1 %=      10000; break;
			case  4: d = p1 /       1000; p1 %=       1000; break;
			case  3: d = p1 /        100; p1 %=        100; break;
			case  2: d = p1 /         10; p1 %=         10; break;
			case  1: d = p1;              p1 =           0; break;
			default: d = 0;
		}
        /* clang-format on */

        if (d || *len)
        {
            buffer[(*len)++] = '0' + cast(char, d);
        }

        kappa--;

        u64 tmp = ((cast(u64, p1)) << -one.e) + p2;
        if (tmp <= delta)
        {
            *K += kappa;
            grisu2_round(buffer,
                         *len,
                         delta,
                         tmp,
                         (cast(u64, pow_10[kappa])) << -one.e,
                         wp_w.f);
            return;
        }
    }

    for (;;)
    {
        p2 *= 10;
        delta *= 10;
        char d = cast(char, p2 >> -one.e);
        if (d || *len)
        {
            buffer[(*len)++] = '0' + d;
        }

        p2 &= one.f - 1;
        kappa--;
        if (p2 < delta)
        {
            *K += kappa;
            grisu2_round(buffer,
                         *len,
                         delta,
                         p2,
                         one.f,
                         wp_w.f * pow_10[-kappa]);
            return;
        }
    }
}

inline void
grisu2(r64 value, char *buffer, s32 *length, s32 *K)
{
    if (value == 0)
    {
        *length = 1;
        *K = 1;
        buffer[0] = '0';
        return;
    }

    struct fp w_m, w_p;
    struct fp v = fp_from_r64(value);

    fp_normalize_boundaries(v, &w_m, &w_p);

    struct fp c_mk = fp_get_cached_power(w_p.e, K);
    struct fp w = fp_normalize(v);
    struct fp W = fp_multiply(w, c_mk);
    struct fp Wp = fp_multiply(w_p, c_mk);
    struct fp Wm = fp_multiply(w_m, c_mk);

    Wm.f++;
    Wp.f--;
    grisu2_digit_gen(W, Wp, Wp.f - Wm.f, buffer, length, K);
}

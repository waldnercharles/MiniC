#include "minic/timer.h"

#include "minic/int.h"
#include "win32/misc.h"

struct timer
{
    LARGE_INTEGER start;
    LARGE_INTEGER frequency;
};

static u64
_u64_muldiv(u64 value, u64 numer, u64 denom)
{
    u64 q = value / denom;
    u64 r = value % denom;
    return q * numer + r * numer / denom;
}

void
timer_init(struct timer *timer)
{
    QueryPerformanceFrequency(&(timer->frequency));
    QueryPerformanceCounter(&(timer->start));
}

u64
timer_now(struct timer *timer)
{
    u64 curr, prev, freq;
    LARGE_INTEGER qpc;

    QueryPerformanceCounter(&qpc);

    curr = cast(u64, qpc.QuadPart);
    prev = cast(u64, timer->start.QuadPart);
    freq = cast(u64, timer->frequency.QuadPart);

    assert(curr >= prev);
    return _u64_muldiv(curr - prev, 1000000000ull, freq);
}

#include "minic/timer.h"

#include "minic/int.h"
#include "win32/misc.h"

struct timer
{
    LARGE_INTEGER start;
    LARGE_INTEGER frequency;
};

static s64
_u64_muldiv(s64 value, s64 numer, s64 denom)
{
    s64 q = value / denom;
    s64 r = value % denom;
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
    s64 curr, prev, freq;
    LARGE_INTEGER qpc;

    QueryPerformanceCounter(&qpc);

    curr = qpc.QuadPart;
    prev = timer->start.QuadPart;
    freq = timer->frequency.QuadPart;

    return _u64_muldiv((curr - prev), 1000000000, freq);
}
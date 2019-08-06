#include "minic/timer.h"

r64
timer_seconds(u64 ticks)
{
    return ticks / 1000000000.0;
}

r64
timer_milliseconds(u64 ticks)
{
    return ticks / 1000000.0;
}

r64
timer_microseconds(u64 ticks)
{
    return ticks / 1000.0;
}

r64
timer_nanoseconds(u64 ticks)
{
    return cast(r64, ticks);
}

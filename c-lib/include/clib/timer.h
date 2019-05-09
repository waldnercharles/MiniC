#ifndef TIMER_H
#define TIMER_H

#include "clib/int.h"

struct timer;

void timer_init(struct timer *timer);
u64 timer_now(struct timer *timer);

r64 timer_seconds(u64 ticks);
r64 timer_milliseconds(u64 ticks);
r64 timer_microseconds(u64 ticks);
r64 timer_nanoseconds(u64 ticks);

#endif
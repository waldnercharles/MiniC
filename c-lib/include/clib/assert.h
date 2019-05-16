#ifndef ASSERT_H
#define ASSERT_H

#include "clib/int.h"

inline void
__assert(bool assertion, const char *message, u32 len);

#define __assert_stringify(x) #x
#define __assert_to_string(x) __assert_stringify(x)
#define __assert_format(x)                                                     \
    "[ASSERT] (" __FILE__ ":" __assert_to_string(__LINE__) "): Assertion (" x  \
                                                           ") failed.\n"
#define assert(x)                                                              \
    __assert(x, __assert_format(#x), sizeof(__assert_format(#x))),             \
        __assume(!!(x));

#endif
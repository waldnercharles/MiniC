#pragma once

#include "minic/int.h"

inline void
__assert(bool assertion, const char *message, u32 len);

#define assert_stringify(x) #x
#define assert_to_string(x) assert_stringify(x)
#define assert_format(x)                                                       \
    "[ASSERT] (" __FILE__ ":" assert_to_string(__LINE__) "): Assertion (" x    \
                                                         ") failed.\n"
#define assert(x)                                                              \
    __assert(x, assert_format(#x), sizeof(assert_format(#x))), __assume(!!(x))

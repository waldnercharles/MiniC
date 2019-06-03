#ifndef STRING_H
#define STRING_H

#include "minic/int.h"

usize
string_length(char *str);

usize
string_copy(char *dst, char *src, usize max);

#endif

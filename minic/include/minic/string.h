#pragma once

#include "minic/int.h"

usize
string_length(const char *str);

usize
string_copy(char *dst, const char *src, usize max);

const char *
string_find_char(const char *str, const char c);

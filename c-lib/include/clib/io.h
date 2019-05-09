#ifndef PRINTF_H
#define PRINTF_H

#include "clib/int.h"

void *io_std_output_handle();

char *io_write_tmp(char *buf, void *udata, u32 len);

s32 __vsnprintf(char *buf, u32 capacity, const char *fmt, va_list va);
s32 __vsprintf(char *buf, const char *fmt, va_list va);
s32 __snprintf(char *buf, u32 capacity, const char *fmt, ...);
s32 __sprintf(char *buf, const char *fmt, ...);
s32 __printf(const char *fmt, ...);

#endif
#pragma once

#include "minic/int.h"

void *
io_output_handle(void);

void *
io_error_handle(void);

usize
io_vsnprintf(char *buf, usize capacity, const char *fmt, va_list va);

usize
io_vsprintf(char *buf, const char *fmt, va_list va);

usize
io_snprintf(char *buf, usize capacity, const char *fmt, ...);

usize
io_sprintf(char *buf, const char *fmt, ...);

usize
io_printf(const char *fmt, ...);

usize
io_printf_handle(const char *fmt, void *handle, ...);

#pragma once

#include "minic/int.h"

void *
io_output_handle(void);

void *
io_error_handle(void);

usize
io_vsnprintf(char *buf, usize capacity, char *fmt, va_list va);

usize
io_vsprintf(char *buf, char *fmt, va_list va);

usize
io_snprintf(char *buf, usize capacity, char *fmt, ...);

usize
io_sprintf(char *buf, char *fmt, ...);

usize
io_printf(char *fmt, ...);

usize
io_printf_handle(char *fmt, void *handle, ...);

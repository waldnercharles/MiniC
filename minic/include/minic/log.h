#pragma once

#include "minic/assert.h"
#include "minic/io.h"

#define log_level_none 0
#define log_level_error 1
#define log_level_warning 2
#define log_level_info 3
#define log_level_debug 4

#ifndef log_level
#define log_level log_level_debug
#endif

void *
log_handle(void);

#define log(message, tag, ...)                                                 \
    io_printf_handle("[%s] (%s:%d): " message "\r\n",                          \
                     log_handle(),                                             \
                     (tag),                                                    \
                     __FILE__,                                                 \
                     __LINE__,                                                 \
                     ##__VA_ARGS__)

#define log_error(message, ...) log(message, "ERROR", ##__VA_ARGS__)
#define log_warning(message, ...) log(message, "WARNING", ##__VA_ARGS__)
#define log_info(message, ...) log(message, "INFO", ##__VA_ARGS__)
#define log_debug(message, ...) log(message, "DEBUG", ##__VA_ARGS__)

#if log_level < log_level_error
#define log_error(message, ...)
#endif

#if log_level < log_level_warning
#define log_warning(message, ...)
#endif

#if log_level < log_level_info
#define log_info(message, ...)
#endif

#if log_level < log_level_debug
#define log_debug(message, ...)
#endif

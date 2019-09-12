#pragma once

#include "minic/assert.h"
#include "minic/io.h"

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_TRACE 5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif

#ifndef LOG_QUIET
#define LOG_QUIET 1
#endif

void *log_handle(void);

#define log(message, tag, ...)                                                 \
    io_printf_handle("[%s] (%s:%d): " message "\r\n",                          \
                     log_handle(),                                             \
                     (tag),                                                    \
                     __FILE__,                                                 \
                     __LINE__,                                                 \
                     ##__VA_ARGS__)

// void log(s32 level, const char *file, s32 line, const char *fmt, ...)
//{
//    if (level < LOG_LEVEL)
//    {
//        return;
//    }
//
//    // TODO: Locking
//
//    if (!LOG_QUIET)
//    {
//        va_list args;
//        char buf[16];
//
//		(void)args
//    }
//}

#define log_error(message, ...) log(message, "ERROR", ##__VA_ARGS__)
#define log_warning(message, ...) log(message, "WARNING", ##__VA_ARGS__)
#define log_info(message, ...) log(message, "INFO", ##__VA_ARGS__)
#define log_debug(message, ...) log(message, "DEBUG", ##__VA_ARGS__)

#if LOG_LEVEL < LOG_LEVEL_ERROR
#undef log_error
#define log_error(message, ...)
#endif

#if LOG_LEVEL < LOG_LEVEL_WARNING
#undef log_warning
#define log_warning(message, ...)
#endif

#if LOG_LEVEL < LOG_LEVEL_INFO
#undef log_info
#define log_info(message, ...)
#endif

#if LOG_LEVEL < LOG_LEVEL_DEBUG
#undef log_debug
#define log_debug(message, ...)
#endif

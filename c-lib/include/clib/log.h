#ifndef LOG_H
#define LOG_H

#include <string.h>

#define log_level_none 0
#define log_level_error 1
#define log_level_warning 2
#define log_level_debug 3

#define log_error_tag "ERROR"
#define log_warning_tag "WARN"
#define log_debug_tag "DEBUG"

#ifndef log_level
#define log_level log_level_debug
#endif

#define log_newline "\n"
#define log_format "[%s] (%s:%s:%d) "
#define log_args(tag) tag, __FILE__, __FUNCTION__, __LINE__

#include <stdio.h>
putchar()

#if log_level >= log_level_error
#define log_error(message, ...)                                                \
    do                                                                         \
    {                                                                          \
        fprintf(stderr,                                                        \
                log_format message log_newline,                                \
                log_args(log_error_tag),                                       \
                ##__VA_ARGS__);                                                \
        fprintf(stdout, "Press any key to continue...");                       \
        getchar();                                                             \
        exit(EXIT_FAILURE);                                                    \
    } while (0)
#else
#define log_error(message, ....)
#endif

#if log_level >= log_level_warning
#define log_warning(message, ...)                                              \
    fprintf(stdout,                                                            \
            log_format message log_newline,                                    \
            log_args(log_warning_tag),                                         \
            ##__VA_ARGS__)
#else
#define log_warning(message, ...)
#endif

#if log_level >= log_level_debug
#define log_debug(message, ...)                                                \
    fprintf(stdout,                                                            \
            log_format message log_newline,                                    \
            log_args(log_debug_tag),                                           \
            ##__VA_ARGS__)
#else
#define log_debug(message, ...)
#endif

#endif
#pragma once

#if defined(__ppc64__) || defined(__aarch64__) || defined(_M_X64) ||           \
    defined(__x86_64__) || defined(__x86_64) || defined(_WIN64)
#define MINIC64
#else
#define MINIC32
#endif

#ifndef __has_attribute
#define __has_attribute(A) 0
#endif

#if __has_attribute(overloadable)
#define overload __attribute__((overloadable))
#else
#define overload
#endif

#define cast(T, V) ((T)(V))
#define NULL cast(void *, 0)

#include <vadefs.h>

#define va_start __crt_va_start
#define va_arg __crt_va_arg
#define va_end __crt_va_end
#define va_copy(destination, source) ((destination) = (source))

/* clang-format off */
/* 1300 == VC 6.0 */
#if defined(_MSC_VER) && _MSC_VER >= 1300
typedef signed __int8       s8;
typedef unsigned __int8     u8;
typedef signed __int16      s16;
typedef unsigned __int16    u16;
typedef signed __int32      s32;
typedef unsigned __int32    u32;
typedef signed __int64      s64;
typedef unsigned __int64    u64;
#else
typedef signed char         s8;
typedef unsigned char       u8;
typedef signed short        s16;
typedef unsigned short      u16;
typedef signed int          s32;
typedef unsigned int        u32;
typedef signed long long    s64;
typedef unsigned long long  u64;
#endif

typedef float               r32;
typedef double              r64;

typedef enum { false, true } bool;
#define true 1
#define false 0

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
/* clang-format on */

#define S8_MIN -0X7F
#define S8_MAX +0X7F
#define U8_MAX +0XFF
#define S16_MIN -0X7FFF
#define S16_MAX +0X7FFF
#define U16_MAX +0XFFFF
#define S32_MIN -0X7FFFFFFF
#define S32_MAX +0X7FFFFFFF
#define U32_MAX +0XFFFFFFFF
#define S64_MIN -0X7FFFFFFFFFFFFFFF
#define S64_MAX +0X7FFFFFFFFFFFFFFF
#define U64_MAX +0XFFFFFFFFFFFFFFFF

#if defined(MINIC64)
typedef s64 sptr;
typedef u64 uptr;
typedef s64 ssize;
typedef u64 usize;
#else
typedef s32 sptr;
typedef u32 uptr;
typedef s32 ssize;
typedef u32 usize;
#endif

typedef union
{
    char *str;

    s8 *s8;
    u8 *u8;

    s16 *s16;
    u16 *u16;

    s32 *s32;
    u32 *u32;

    s64 *s64;
    u64 *u64;

} RawBuffer;

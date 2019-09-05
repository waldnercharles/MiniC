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

#ifdef __cplusplus
#define NULL nullptr

#define cast(T, V) (static_cast<T>(V))
#define unsafe_cast(T, V) (reinterpret_cast<T>(V))
#else
#define NULL cast(void *, 0)
#define cast(T, V) ((T)(V))
#define unsafe_cast cast
#endif

#include <vadefs.h>

#define va_start __crt_va_start
#define va_end __crt_va_end
#define va_arg __crt_va_arg

#if defined(__GNU_C__) || defined(__clang__)
#ifdef _crt_va_start
#undef _crt_va_start
#define _crt_va_start(ap, param) __builtin_va_start(ap, param)
#endif
#ifdef _crt_va_end
#undef _crt_va_end
#define _crt_va_end(ap) __builtin_va_end(ap)
#endif
#ifdef _crt_va_arg
#undef _crt_va_arg
#define _crt_va_arg(ap, type) __builtin_va_arg(ap, type)
#endif

#ifdef __crt_va_start
#undef __crt_va_start
#define __crt_va_start(ap, param) __builtin_va_start(ap, param)
#endif
#ifdef __crt_va_end
#undef __crt_va_end
#define __crt_va_end(ap) __builtin_va_end(ap)
#endif
#ifdef __crt_va_arg
#undef __crt_va_arg
#define __crt_va_arg(ap, type) __builtin_va_arg(ap, type)
#endif
#endif

/* clang-format off */
typedef signed char         s8;
typedef unsigned char       u8;
typedef signed short        s16;
typedef unsigned short      u16;
typedef signed int          s32;
typedef unsigned int        u32;
typedef signed long long    s64;
typedef unsigned long long  u64;

typedef	wchar_t				wchar;

typedef float               r32;
typedef double              r64;

// typedef enum { false, true } bool;
// #define true 1
// #define false 0

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

typedef union
{
    const char *str;

    const s8 *s8;
    const u8 *u8;

    const s16 *s16;
    const u16 *u16;

    const s32 *s32;
    const u32 *u32;

    const s64 *s64;
    const u64 *u64;
} ReadOnlyRawBuffer;

template <typename T>
struct remove_reference
{
    typedef T type;
};

template <typename T>
struct remove_reference<T &>
{
    typedef T type;
};

template <typename T>
struct remove_reference<T &&>
{
    typedef T type;
};

template <typename T>
typename remove_reference<T>::type &&rvalue(T &&arg)
{
    return static_cast<typename remove_reference<decltype(arg)>::type &&>(arg);
}

template <typename T>
void swap(T &x, T &y)
{
    T tmp = rvalue(x);
    x = rvalue(y);
    y = rvalue(tmp);
}

template <typename TIn, typename TOut>
TOut copy(TIn first, TIn last, TOut result)
{
    while (first != last)
    {
        *result++ = *first++;
    }

    return result;
}

#ifndef INT_H
#define INT_H

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
#if defined(_MSC_VER)
/* 1300 == VC 6.0 */
#if _MSC_VER < 1300
typedef signed char         s8;
typedef unsigned char       u8;
typedef signed short        s16;
typedef unsigned short      u16;
typedef signed int          s32;
typedef unsigned int        u32;
#else
typedef signed __int8       s8;
typedef unsigned __int8     u8;
typedef signed __int16      s16;
typedef unsigned __int16    u16;
typedef signed __int32      s32;
typedef unsigned __int32    u32;
#endif
typedef signed __int64      s64;
typedef unsigned __int64    u64;
#else
typedef signed char         s8;
typedef short               u8;
typedef int                 s16;
typedef long long           u16;
typedef unsigned char       s32;
typedef unsigned short      u32;
typedef unsigned int        s64;
typedef unsigned long long  u64;
#endif

typedef float               r32;
typedef double              r64;

typedef enum { false, true } bool;

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

const u64 U32_MAX_VALUE = 0xffffffff;
const u64 U64_MAX_VALUE = (0xffffffffull << 32) + 0xffffffffull;

#if defined(__ppc64__) || defined(__aarch64__) || defined(_M_X64) ||           \
    defined(__x86_64__) || defined(__x86_64)
typedef uintptr_t ptr_t;
typedef u64 size_t;
#else
typedef u32 ptr_t;
typedef u32 size_t;
#endif

#endif
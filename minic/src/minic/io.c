#include "minic/assert.h"
#include "minic/fp.h"
#include "minic/int.h"
#include "minic/io.h"
#include "minic/math.h"
#include "minic/memory.h"

#define FLAGS_LEFT 1u
#define FLAGS_PLUS 2u
#define FLAGS_SPACE 4u
#define FLAGS_HASH 8u
#define FLAGS_ZERO 16u
#define FLAGS_HALF 32u
#define FLAGS_LONG_LONG 64u
#define FLAGS_NEGATIVE 128u

typedef char *
io_write_fn(char *buf, void *udata, usize len);

struct io_vsnprintf_context
{
    usize cap;
    char *buf;
    char tmp[512];
};

struct io_write_context
{
    io_write_fn *out;
    void *udata;

    /* Ring buffer */
    RawBuffer tail;
    RawBuffer head;
    usize bytes_written;
};

struct io_specifier_context
{
    char specifier;

    u32 flags;
    usize width;
    usize precision;

    char tmp[512];

    char *str, pre[8], post[8];
    usize str_len, pre_len, post_len;
};

static void
io_write(struct io_write_context *p, usize nbytes)
{
    if (p->out)
    {
        assert(p->head.str >= p->tail.str);
        usize len = cast(usize, p->head.str - p->tail.str);
        if ((len + nbytes) >= 512)
        {
            p->bytes_written += len;
            p->head.str = p->tail.str = p->out(p->tail.str, p->udata, len);
        }
    }
}

static void
io_flush(struct io_write_context *p)
{
    io_write(p, 512 - 1);
}

static char *
io_write_until_next_specifier(char *fmt, struct io_write_context *p)
{
    RawBuffer buf = { .str = fmt };

    // Align to a multiple of 4
    while ((cast(uptr, buf.str) & 3))
    {
    percent_check:
        if (*buf.str == '%')
        {
            return buf.str;
        }
    null_check:
        if (*buf.str == 0)
        {
            return buf.str;
        }
        io_write(p, 1);
        *p->head.u8++ = *buf.u8;
        ++buf.u8;
    }
    for (;;)
    {
        // Check if the next 4 bytes contain % (0x25) or NULL (0x00)
        // https://graphics.stanford.edu/~seander/bithacks.html#HasLessInWord
        u32 word = *buf.u32;
        u32 mask = (~word) & 0x80808080;
        if (((word ^ 0x25252525) - 0x01010101) & mask)
        {
            goto percent_check;
        }
        if ((word - 0x01010101) & mask)
        {
            goto null_check;
        }
        if (p->out && ((512 - (p->head.str - p->tail.str)) < 4))
        {
            goto percent_check;
        }

        *p->head.u32 = word;

        p->head.str += 4;
        buf.str += 4;
    }
}

static char *
io_parse_flags(char *fmt, u32 *flags)
{
    *flags = 0;
    for (;;)
    {
        /* clang-format off */
        switch (*fmt)
        {
            case '-': { *flags |= FLAGS_LEFT;  ++fmt; continue; }
            case '+': { *flags |= FLAGS_PLUS;  ++fmt; continue; }
            case ' ': { *flags |= FLAGS_SPACE; ++fmt; continue; }
            case '#': { *flags |= FLAGS_HASH;  ++fmt; continue; }
            case '0': { *flags |= FLAGS_ZERO;  ++fmt; continue; }
            default:  { return fmt; }
        }
        /* clang-format on */
    }
}

static char *
io_parse_width(char *fmt, usize *width, va_list *va)
{
    *width = 0;
    if (*fmt == '*')
    {
        *width = va_arg(*va, usize);
        ++fmt;
    }
    else
    {
        while (*fmt >= '0' && *fmt <= '9')
        {
            *width = *width * 10 + *cast(u8 *, fmt) - '0';
            ++fmt;
        }
    }

    return fmt;
}

static char *
io_parse_precision(char *fmt, usize *precision, va_list *va)
{
    if (*fmt == '.')
    {
        ++fmt;
        if (*fmt == '*')
        {
            *precision = va_arg(*va, usize);
            ++fmt;
        }
        else
        {
            *precision = 0;
            while (*fmt >= '0' && *fmt <= '9')
            {
                *precision = *precision * 10 + *cast(u8 *, fmt) - '0';
                ++fmt;
            }
        }
    }
    else
    {
        *precision = 0xffffffff;
    }

    return fmt;
}

static char *
io_parse_int_length(char *fmt, u32 *flags)
{
    switch (*fmt)
    {
        /* Half-width */
        case 'h':
        {
            *flags |= FLAGS_HALF;
            ++fmt;
            break;
        }
        case 'l':
        {
            ++fmt;
            if (*fmt == 'l')
            {
                *flags |= FLAGS_LONG_LONG;
                ++fmt;
            }
            break;
        }
        /* 64-bit size_t or ptrdiff_t */
        case 'z':
        case 't':
        {
            *flags |= ((sizeof(void *) == 8) ? FLAGS_LONG_LONG : 0);
            ++fmt;
        }
        /* 64-bit (msft style) */
        case 'I':
        {
            if ((fmt[1] == '6') && (fmt[2] == '4'))
            {
                *flags |= FLAGS_LONG_LONG;
                fmt += 3;
            }
            else if ((fmt[1] == '3') && (fmt[2] == '2'))
            {
                fmt += 3;
            }
            else
            {
                *flags |= ((sizeof(void *) == 8) ? FLAGS_LONG_LONG : 0);
                ++fmt;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return fmt;
}

static usize
io_strlen_clamp(char *str, usize clamp_to)
{
    usize len;
    RawBuffer buf = { .str = str };

    while ((cast(uptr, buf.str)) & 3)
    {
    null_check:
        if (*buf.str == 0)
        {
            assert(buf.str >= str);
            len = cast(usize, buf.str - str);
            return min(len, clamp_to);
        }
        ++buf.u8;
    }

    assert(buf.str >= str);
    len = cast(usize, buf.str - str);
    if (len >= clamp_to)
    {
        return clamp_to;
    }

    assert(clamp_to >= len);
    usize word_count = (clamp_to - len) >> 2;

    while (word_count--)
    {
        u32 word = *buf.u32;
        if ((word - 0x01010101) & (~word) & 0x80808080)
        {
            goto null_check;
        }
        buf.u8 += 4;
    }
    goto null_check;
}

static void
io_write_to_context(struct io_specifier_context *s, struct io_write_context *w)
{
    RawBuffer str_buf = { .str = s->str };
    char *pre = s->pre;
    char *post = s->post;
    usize str_len = s->str_len;
    usize pre_len = s->pre_len;
    usize post_len = s->post_len;

    // TODO: Handle leading spaces
    while (pre_len)  // copy prefix
    {
        assert(w->head.str >= w->tail.str);
        usize n = min(pre_len, 512 - cast(usize, w->head.str - w->tail.str));
        pre_len -= n;
        while (n)
        {
            *w->head.str++ = *pre++;
            --n;
        }

        io_write(w, 1);
    }

    // TODO: Handle leading zeros

    while (str_len)  // copy src
    {
        assert(w->head.str >= w->tail.str);
        usize n = min(str_len, 512 - cast(usize, w->head.str - w->tail.str));

        str_len -= n;
        while (n >= 4)
        {
            // Copy 4 bytes at a time
            *w->head.u32 = *str_buf.u32;
            w->head.str += 4;
            str_buf.str += 4;
            n -= 4;
        }
        while (n > 0)
        {
            *w->head.u8++ = *str_buf.u8++;
            --n;
        }

        io_write(w, 1);
    }

    // TODO: Handle trailing zeros

    while (post_len)  // copy postfix
    {
        assert(w->head.str >= w->tail.str);
        usize n = min(post_len, 512 - cast(usize, w->head.str - w->tail.str));
        post_len -= n;
        while (n)
        {
            *w->head.str++ = *(post)++;
            --n;
        }

        io_write(w, 1);
    }

    // TODO: Handle left-justify
}

static void
io_parse_base2(struct io_specifier_context *s, u32 pow, char *hex, va_list *va)
{
    u64 num;
    if (s->flags & FLAGS_LONG_LONG)
    {
        num = va_arg(*va, u64);
    }
    else
    {
        num = va_arg(*va, u32);
    }

    /* Convert to string */
    s->str = s->tmp + 512;
    for (;;)
    {
        *--(s->str) = hex[num & ((1 << pow) - 1)];
        num >>= pow;
        if (num == 0 || cast(u32, (s->tmp + 512) - s->str) >= s->precision)
        {
            break;
        }
    }

    s->str_len = cast(u32, (s->tmp + 512) - s->str);
}

static void
io_parse_string(struct io_specifier_context *s, va_list *va)
{
    s->str = va_arg(*va, char *);
    if (s->str == NULL)
    {
        s->str = "null";
    }

    s->str_len = io_strlen_clamp(s->str, s->precision);
}

static inline void
io_parse_char(struct io_specifier_context *s, va_list *va)
{
    s->tmp[0] = cast(char, va_arg(*va, u32));
    s->tmp[1] = 0;

    s->str = s->tmp;
    s->str_len = 1;
}

static inline void
io_parse_sign(struct io_specifier_context *s)
{
    s->pre_len = 0;
    if (s->flags & FLAGS_NEGATIVE)
    {
        s->pre[0] = '-';
        s->pre_len = 1;
    }
    else if (s->flags & FLAGS_SPACE)
    {
        s->pre[0] = ' ';
        s->pre_len = 1;
    }
    else if (s->flags & FLAGS_PLUS)
    {
        s->pre[0] = '+';
        s->pre_len = 1;
    }
}

static inline void
io_parse_base10(struct io_specifier_context *s, va_list *va)
{
    u64 num;

    if (s->flags & FLAGS_LONG_LONG)
    {
        s64 i = va_arg(*va, s64);
        if ((s->specifier != 'u') && (i < 0))
        {
            num = cast(u64, -i);
            s->flags |= FLAGS_NEGATIVE;
        }
        else
        {
            num = cast(u64, i);
        }
    }
    else
    {
        s32 i = va_arg(*va, s32);
        if ((s->specifier != 'u') && (i < 0))
        {
            num = cast(u32, -i);
            s->flags |= FLAGS_NEGATIVE;
        }
        else
        {
            num = cast(u32, i);
        }
    }

    /* Convert to string */
    s->str = s->tmp + 512;
    s->str_len = 0;

    for (;;)
    {
        /* 32 bit chunks at a time */
        char *ptr = s->str - 8;
        u32 n;
        if (num >= 100000000)
        {
            n = cast(u32, num % 100000000);
            num /= 100000000;
        }
        else
        {
            n = cast(u32, num);
            num = 0;
        }

        while (n)
        {
            *--(s->str) = (n % 10) + '0';
            n /= 10;
        }

        if (num == 0)
        {
            if ((s->str[0] == '0') && (s->str != (s->tmp + 512)))
            {
                ++(s->str);
            }
            break;
        }

        while (s->str != ptr)
        {
            *--(s->str) = '0';
        }
    }

    io_parse_sign(s);

    s->str_len = cast(u32, (s->tmp + 512) - s->str);
    if (s->str_len == 0)
    {
        *--(s->str) = '0';
        s->str_len = 1;
    }
}

static inline void
io_parse_float(struct io_specifier_context *s, va_list *va)
{
    r64 value = va_arg(*va, r64);

    if (s->precision == 0xffffffff)
    {
        s->precision = 6;  // Default is 6.
    }

    if (value < 0)
    {
        s->flags |= FLAGS_NEGATIVE;
        value = -value;
    }

    if (r64_isnan(value))
    {
        s->str = "NaN";
        s->str_len = 3;
        return;
    }
    if (r64_isinf(value))
    {
        s->str = "Inf";
        s->str_len = 3;
        return;
    }

    ssize length;
    s32 K;
    grisu2(value, s->tmp, cast(usize *, &length), &K);

    ssize kk = length + K;
    /* 21 is just a number. Only 16 digits are possibly relevant */
    if (length <= kk && kk <= 21)
    {
        /* 1234e7 -> 12340000000 */
        for (ssize i = length; i < kk; i++)
        {
            s->tmp[i] = '0';
        }
        s->tmp[kk] = '.';
        s->tmp[kk + 1] = '0';
        s->tmp[kk + 2] = '\0';

        length += (kk + 2);
    }
    else if (0 < kk && kk <= 21)
    {
        /* Exponent is small enough to just insert a '.' */
        /* 1234e-2 -> 12.34 */

        assert(length > 0);
        assert(length >= kk);
        mem_move(&s->tmp[kk + 1], &s->tmp[kk], cast(usize, length - kk));

        s->tmp[kk] = '.';
        s->tmp[length + 1] = '\0';

        length += 1;
    }
    else if (-6 < kk && kk <= 0)
    {
        /* 1234e-6 -> 0.001234 */
        const ssize offset = 2 - kk;

        assert(length > 0);
        mem_move(&s->tmp[offset], &s->tmp[0], cast(usize, length));

        s->tmp[0] = '0';
        s->tmp[1] = '.';

        for (ssize i = 2; i < offset; i++)
        {
            s->tmp[i] = '0';
        }
        s->tmp[length + offset] = '\0';

        length += (2 - kk);
    }
    else
    {
        char *e_buffer;
        if (length == 1)
        {
            /* No decimal at all. Just add 'e...' */
            /* 1e30 */
            e_buffer = &s->tmp[2];

            s->tmp[1] = 'e';
            length += 1;
        }
        else
        {
            /* 1234e30 -> 1.234e33 */
            assert(length > 0);
            mem_move(&s->tmp[2], &s->tmp[1], cast(usize, length - 1));

            e_buffer = &s->tmp[length + 2];

            s->tmp[1] = '.';
            s->tmp[length + 1] = 'e';
            length += 2;
        }

        /* Write exponent */
        kk -= 1;
        static const char digits_lut[200] = {
            '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0',
            '6', '0', '7', '0', '8', '0', '9', '1', '0', '1', '1', '1', '2',
            '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1',
            '9', '2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5',
            '2', '6', '2', '7', '2', '8', '2', '9', '3', '0', '3', '1', '3',
            '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8',
            '3', '9', '4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4',
            '5', '4', '6', '4', '7', '4', '8', '4', '9', '5', '0', '5', '1',
            '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5',
            '8', '5', '9', '6', '0', '6', '1', '6', '2', '6', '3', '6', '4',
            '6', '5', '6', '6', '6', '7', '6', '8', '6', '9', '7', '0', '7',
            '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7',
            '7', '8', '7', '9', '8', '0', '8', '1', '8', '2', '8', '3', '8',
            '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9', '9', '0',
            '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9',
            '7', '9', '8', '9', '9'
        };

        if (kk < 0)
        {
            *e_buffer++ = '-';
            kk = -kk;
            length += 1;
        }

        if (kk >= 100)
        {
            *e_buffer++ = cast(char, '0' + (kk / 100));
            kk %= 100;

            const char *digits = digits_lut + kk * 2;
            *e_buffer++ = digits[0];
            *e_buffer++ = digits[1];
            length += 3;
        }
        else if (kk >= 10)
        {
            const char *digits = digits_lut + kk * 2;
            *e_buffer++ = digits[0];
            *e_buffer++ = digits[1];
            length += 2;
        }
        else
        {
            *e_buffer++ = cast(char, '0' + kk);
            length += 1;
        }

        *e_buffer = '\0';
    }

    s->str = s->tmp;

    assert(length > 0);
    s->str_len = cast(usize, length);
}

static inline void
io_parse_default(struct io_specifier_context *s)
{
    s->tmp[0] = '%';
    s->tmp[1] = s->specifier;
    s->tmp[2] = 0;

    s->str = s->tmp;
    s->str_len = 2;
}

static char *
io_write_specifier(char *fmt, struct io_write_context *w, va_list *va)
{
    struct io_specifier_context s = { 0 };

    /* Handle flags, width, precision, and length */
    fmt = io_parse_flags(fmt, &s.flags);
    fmt = io_parse_width(fmt, &s.width, va);
    fmt = io_parse_precision(fmt, &s.precision, va);
    fmt = io_parse_int_length(fmt, &s.flags);

    /* Get specifier after reading the flags, width, precision, and length */
    s.specifier = *fmt;

    switch (s.specifier)
    {
        static char hex_l[] = "0123456789abcdefxp";
        static char hex_u[] = "0123456789ABCDEFXP";

        case 's':  // string
        {
            io_parse_string(&s, va);
            break;
        }
        case 'c':  // char
        {
            io_parse_char(&s, va);
            break;
        }
        case 'B':  // uppercase binary
        case 'b':  // lowercase binary
        {
            char *hex = s.specifier == 'B' ? hex_u : hex_l;
            if (s.flags & FLAGS_HASH)
            {
                s.pre[0] = '0';
                s.pre[1] = hex[0xB];
                s.pre_len = 2;
            }

            io_parse_base2(&s, 1, hex, va);
            break;
        }
        case 'o':  // octal
        {
            char *hex = hex_u;
            if (s.flags & FLAGS_HASH)
            {
                s.pre[0] = '0';
                s.pre_len = 1;
            }
            io_parse_base2(&s, 3, hex, va);
            break;
        }
        case 'p':  // pointer
        {
            s.flags |= (sizeof(uptr) == 8) ? FLAGS_LONG_LONG : 0;
            s.flags &= ~FLAGS_ZERO;

            s.precision = sizeof(uptr) * 2;

            // Fall through to hex.
        }
        case 'X':  // uppercase hex
        case 'x':  // lowercase hex
        {
            char *hex = s.specifier == 'X' ? hex_u : hex_l;
            if (s.flags & FLAGS_HASH)
            {
                s.pre[0] = '0';
                s.pre[1] = hex[16];
                s.pre_len = 2;
            }
            io_parse_base2(&s, 4, hex, va);
            break;
        }
        case 'u':
        case 'i':
        case 'd':
        {
            io_parse_base10(&s, va);
            break;
        }
        case 'f':
        {
            io_parse_float(&s, va);
            break;
        }
        default:
        {
            io_parse_default(&s);
            break;
        }
    }

    io_write_to_context(&s, w);

    return fmt + 1;
}

static usize
io_vsprintf_cb(io_write_fn *out, void *udata, char *buf, char *fmt, va_list va)
{
    struct io_write_context w;
    w.out = out;
    w.udata = udata;
    w.head.str = w.tail.str = buf;
    w.bytes_written = 0;

    for (;;)
    {
        fmt = io_write_until_next_specifier(fmt, &w);
        if (*fmt == 0) /* Handle null-terminator */
        {
            goto endfmt;
        }
        else /* Handle percent */
        {
            ++fmt;
        }

        /* Handle specifier */
        fmt = io_write_specifier(fmt, &w, &va);
    }

endfmt:
    if (w.out == NULL)
    {
        *(w.head.str) = 0;
    }
    else
    {
        io_flush(&w);
    }

    assert(w.head.str >= w.tail.str);
    return w.bytes_written + cast(usize, w.head.str - w.tail.str);
}

static char *
io_vsnprintf_write(char *buf, void *udata, usize len)
{
    struct io_vsnprintf_context *p = udata;

    if (len > p->cap)
    {
        len = p->cap;
    }

    if (len)
    {
        if (buf != p->buf)
        {
            char *dest, *src, *src_end;

            dest = p->buf;
            src = buf;
            src_end = buf + len;

            while (src < src_end)
            {
                *dest++ = *src++;
            }

            p->buf += len;
            p->cap -= len;
        }
    }

    if (p->cap <= 0)
    {
        return 0;
    }

    // Write to tmp buffer until our count is less than the write threshold.
    return p->cap >= 512 ? p->buf : p->tmp;
}

static char *
io_vsnprintf_count(char *buf, void *udata, usize len)
{
    cast(void, buf);

    struct io_vsnprintf_context *p = udata;
    p->cap += len;

    return p->tmp;
}

usize
io_vsnprintf(char *buf, usize capacity, char *fmt, va_list va)
{
    struct io_vsnprintf_context p;
    usize len;
    if (capacity == 0 && buf == NULL)
    {
        p.cap = 0;
        io_vsprintf_cb(io_vsnprintf_count, &p, p.tmp, fmt, va);
        len = p.cap;
    }
    else
    {
        if (capacity == 0)
        {
            return 0;
        }

        p.buf = buf;
        p.cap = capacity;

        io_vsprintf_cb(io_vsnprintf_write,
                       &p,
                       io_vsnprintf_write(0, &p, 0),
                       fmt,
                       va);

        // Zero-terminate
        assert(p.buf >= buf);
        len = min(cast(usize, p.buf - buf), capacity - 1);

        buf[len] = 0;
    }

    return len;
}

usize
io_vsprintf(char *buf, char *fmt, va_list va)
{
    struct io_write_context p;

    p.out = NULL;
    p.udata = NULL;
    p.head.str = buf;
    p.tail.str = buf;

    return io_vsprintf_cb(NULL, NULL, buf, fmt, va);
}

usize
io_snprintf(char *buf, usize capacity, char *fmt, ...)
{
    usize result;
    va_list va;

    va_start(va, fmt);
    result = io_vsnprintf(buf, capacity, fmt, va);
    va_end(va);

    return result;
}

usize
io_sprintf(char *buf, char *fmt, ...)
{
    usize result;
    va_list va;

    va_start(va, fmt);
    result = io_vsprintf_cb(NULL, NULL, buf, fmt, va);
    va_end(va);

    return result;
}

char *
io_write_tmp(char *buf, void *udata, usize len);

usize
io_printf_handle(char *fmt, void *handle, ...)
{
    usize result;
    va_list va;

    char tmp[512];

    va_start(va, handle);
    result = io_vsprintf_cb(io_write_tmp, handle, tmp, fmt, va);
    va_end(va);

    return result;
}

usize
io_printf(char *fmt, ...)
{
    usize result;
    va_list va;

    char tmp[512];

    va_start(va, fmt);
    result = io_vsprintf_cb(io_write_tmp, io_output_handle(), tmp, fmt, va);
    va_end(va);

    return result;
}

#include "clib/io.h"

#include "clib/int.h"
#include "clib/math.h"
#include "clib/memory.h"
#include "clib/fp.h"

#define CPW_IO_MIN_PRINT_LEN 512

#define FLAGS_LEFT 1
#define FLAGS_PLUS 2
#define FLAGS_SPACE 4
#define FLAGS_HASH 8
#define FLAGS_ZERO 16
#define FLAGS_HALF 32
#define FLAGS_LONG_LONG 64
#define FLAGS_NEGATIVE 128

typedef char *io_write_fn(char *buf, void *udata, u32 len);

struct io_vsnprintf_context
{
    u32 cap;
    char *buf;
    char tmp[512];
};

struct io_write_context
{
    io_write_fn *out;
    void *udata;

    /* Ring buffer */
    char *tail;
    char *head;
    u32 bytes_written;
};

struct io_specifier_context
{
    char specifier;

    u32 flags;
    u32 width;
    u32 precision;

    char tmp[512];

    char *str, pre[8], post[8];
    u32 str_len, pre_len, post_len;
};

static void
io_write(struct io_write_context *p, u32 nbytes)
{
    if (p->out)
    {
        u32 len = cast(s32, p->head - p->tail);
        if ((len + nbytes) >= 512)
        {
            p->bytes_written += len;
            p->head = p->tail = p->out(p->tail, p->udata, len);
        }
    }
};

static void
io_flush(struct io_write_context *p)
{
    io_write(p, 512 - 1);
}

static const char *
io_write_until_next_specifier(const char *fmt, struct io_write_context *p)
{
    // Align to a multiple of 4
    while ((cast(ptr_t, fmt) & 3)
    {
    percent_check:
        if (*fmt == '%')
        {
            return fmt;
        }
    null_check:
        if (*fmt == 0)
        {
            return fmt;
        }
        io_write(p, 1);
        *(p->head)++ = *fmt;
        ++fmt;
    }
    for (;;)
    {
        // Check if the next 4 bytes contain % (0x25) or NULL (0x00)
        // https://graphics.stanford.edu/~seander/bithacks.html#HasLessInWord
        u32 word = *cast(u32 *, fmt);
        u32 mask = (~word) & 0x80808080;
        if (((word ^ 0x25252525) - 0x01010101) & mask)
        {
            goto percent_check;
        }
        if ((word - 0x01010101) & mask)
        {
            goto null_check;
        }
        if (p->out && ((512 - cast(s32, p->head - p->tail)) < 4))
        {
            goto percent_check;
        }
        *cast(u32 *, p->head) = word;
        p->head += 4;
        fmt += 4;
    }
}

static const char *
io_parse_flags(const char *fmt, u32 *flags)
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

static const char *
io_parse_width(const char *fmt, u32 *width, va_list *va)
{
    *width = 0;
    if (*fmt == '*')
    {
        *width = va_arg(*va, u32);
        ++fmt;
    }
    else
    {
        while (*fmt >= '0' && *fmt <= '9')
        {
            *width = *width * 10 + *fmt - '0';
            ++fmt;
        }
    }

    return fmt;
}

static const char *
io_parse_precision(const char *fmt, u32 *precision, va_list *va)
{
    if (*fmt == '.')
    {
        ++fmt;
        if (*fmt == '*')
        {
            *precision = va_arg(*va, u32);
            ++fmt;
        }
        else
        {
            *precision = 0;
            while (*fmt >= '0' && *fmt <= '9')
            {
                *precision = *precision * 10 + *fmt - '0';
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

static const char *
io_parse_int_length(const char *fmt, u32 *flags)
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

u32
io_strlen_clamp(const char *str, u32 clamp_to)
{
    u32 word, word_count, len;
    const char *char_ptr;

    char_ptr = str;
    while ((cast(ptr_t, char_ptr)) & 3)
    {
    null_check:
        if (*char_ptr == 0)
        {
            len = cast(u32, char_ptr - str);
            return min(len, clamp_to);
        }
        ++char_ptr;
    }

    len = cast(u32, char_ptr - str);
    if (len >= clamp_to)
    {
        return clamp_to;
    }
    word_count = (cast(u32, clamp_to - len)) >> 2;

    while (word_count--)
    {
        word = *cast(u32 *, char_ptr);
        if ((word - 0x01010101) & (~word) & 0x80808080)
        {
            goto null_check;
        }
        char_ptr += 4;
    }
    goto null_check;
}

static void
io_write_to_context(struct io_specifier_context *s, struct io_write_context *w)
{
    char *str = s->str;
    char *pre = s->pre;
    char *post = s->post;
    u32 str_len = s->str_len;
    u32 pre_len = s->pre_len;
    u32 post_len = s->post_len;

    // TODO: Handle leading spaces
    while (pre_len)  // copy prefix
    {
        u32 n = min(pre_len, 512 - cast(u32, w->head - w->tail));
        pre_len -= n;
        while (n)
        {
            *w->head++ = *pre++;
            --n;
        }

        io_write(w, 1);
    }

    // TODO: Handle leading zeros

    while (str_len)  // copy src
    {
        u32 n = min(str_len, 512 - cast(u32, w->head - w->tail));
        str_len -= n;
        while (n >= 4)
        {
            // Copy 4 bytes at a time
            *cast(u32 *, w->head) = *cast(u32 *, str);
            w->head += 4;
            str += 4;
            n -= 4;
        }
        while (n > 0)
        {
            *w->head++ = *str++;
            --n;
        }

        io_write(w, 1);
    }

    // TODO: Handle trailing zeros

    while (post_len)  // copy postfix
    {
        u32 n = min(post_len, 512 - cast(u32, w->head - w->tail));
        post_len -= n;
        while (n)
        {
            *w->head++ = *(post)++;
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
            num = -i;
            s->flags |= FLAGS_NEGATIVE;
        }
        else
        {
            num = i;
        }
    }
    else
    {
        s32 i = va_arg(*va, s32);
        if ((s->specifier != 'u') && (i < 0))
        {
            num = -i;
            s->flags |= FLAGS_NEGATIVE;
        }
        else
        {
            num = i;
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
            *--(s->str) = cast(u8, n % 10) + '0';
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

    s32 length, K;
    grisu2(value, s->tmp, &length, &K);

    s32 kk = length + K;
    /* 21 is just a number. Only 16 digits are possibly relevant */
    if (length <= kk && kk <= 21)
    {
        /* 1234e7 -> 12340000000 */
        for (s32 i = length; i < kk; i++)
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
        mem_move(&s->tmp[kk + 1], &s->tmp[kk], length - kk);
        s->tmp[kk] = '.';
        s->tmp[length + 1] = '\0';

        length += 1;
    }
    else if (-6 < kk && kk <= 0)
    {
        /* 1234e-6 -> 0.001234 */
        const s32 offset = 2 - kk;
        mem_move(&s->tmp[offset], &s->tmp[0], length);
        s->tmp[0] = '0';
        s->tmp[1] = '.';

        for (s32 i = 2; i < offset; i++)
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
            mem_move(&s->tmp[2], &s->tmp[1], length - 1);

            e_buffer = &s->tmp[length + 2];

            s->tmp[1] = '.';
            s->tmp[length + 1] = 'e';
            length += 2;
        }

        /* Write exponent */
        kk -= 1;
        static const char digits_lut[200] =
            { '0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0',
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
              '7', '9', '8', '9', '9' };

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
    s->str_len = length;
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

static const char *
io_write_specifier(const char *fmt, struct io_write_context *w, va_list *va)
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
            s.flags |= (sizeof(ptr_t) == 8) ? FLAGS_LONG_LONG : 0;
            s.flags &= ~FLAGS_ZERO;

            s.precision = sizeof(ptr_t) * 2;

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

static s32
io_vsprintf_cb(io_write_fn *out,
               void *udata,
               char *buf,
               const char *fmt,
               va_list va)
{
    struct io_write_context w;
    w.out = out;
    w.udata = udata;
    w.head = w.tail = buf;
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
        *(w.head) = 0;
    }
    else
    {
        io_flush(&w);
    }

    return w.bytes_written + cast(s32, w.head - w.tail);
}

static char *
io_vsnprintf_write(char *buf, void *udata, u32 len)
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
io_vsnprintf_count(char *buf, void *udata, u32 len)
{
    cast(void, buf);

    struct io_vsnprintf_context *p = udata;
    p->cap += len;

    return p->tmp;
}

s32
__vsnprintf(char *buf, u32 capacity, const char *fmt, va_list va)
{
    struct io_vsnprintf_context p;
    u32 len;
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
        len = min(cast(u32, p.buf - buf), capacity - 1);
        buf[len] = 0;
    }

    return len;
}

s32
__vsprintf(char *buf, const char *fmt, va_list va)
{
    struct io_write_context p;

    p.out = NULL;
    p.udata = NULL;
    p.head = buf;
    p.tail = buf;

    return io_vsprintf_cb(NULL, NULL, buf, fmt, va);
}

s32
__snprintf(char *buf, u32 capacity, const char *fmt, ...)
{
    s32 result;
    va_list va;

    va_start(va, fmt);
    result = __vsnprintf(buf, capacity, fmt, va);
    va_end(va);

    return result;
}

s32
__sprintf(char *buf, const char *fmt, ...)
{
    s32 result;
    va_list va;

    va_start(va, fmt);
    result = io_vsprintf_cb(NULL, NULL, buf, fmt, va);
    va_end(va);

    return result;
}

s32
__printf(const char *fmt, ...)
{
    char tmp[512];

    s32 result;
    va_list va;

    va_start(va, fmt);
    result = io_vsprintf_cb(io_write_tmp, io_std_output_handle(), tmp, fmt, va);
    va_end(va);

    return result;
}

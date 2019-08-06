#include "minic/io.h"
#include "minic/assert.h"

#include <win32/io.h>
#include <win32/file.h>

void *io_output_handle(void)
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

void *io_error_handle(void)
{
    return GetStdHandle(STD_ERROR_HANDLE);
}

char *io_write_tmp(char *buf, void *udata, usize len)
{
    assert(len == cast(DWORD, len));

    DWORD bytes_written;
    WriteFile(udata, buf, cast(DWORD, len), &bytes_written, NULL);
    return buf;
}

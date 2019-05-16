#include "minic/io.h"

#include <win32/io.h>
#include <win32/file.h>

void *
io_std_output_handle()
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

char *
io_write_tmp(char *buf, void *udata, u32 len)
{
    DWORD bytes_written;
    WriteFile(udata, buf, len, &bytes_written, NULL);
    return buf;
}
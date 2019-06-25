#include "minic/log.h"

#include <win32/file.h>
#include <win32/io.h>

void *
log_handle(void)
{
    static HANDLE handle = 0;

    if (handle == 0)
    {
        handle = CreateFileA("log.txt",
                             GENERIC_WRITE,
                             FALSE,
                             NULL,
                             CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);

        assert(handle != INVALID_HANDLE_VALUE);
    }

    return handle;
}

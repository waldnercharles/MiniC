#include "minic/assert.h"
#include "minic/int.h"

//#include <Windows.h>
#include <win32/dbghelp.h>
#include <win32/file.h>
#include <win32/io.h>
#include <win32/process.h>
#include <win32/threads.h>

inline void __assert(bool assertion, const char *message, u32 len)
{
#ifdef DEBUG
    if (!assertion)
    {
        DWORD bytes_written;
        HANDLE error_handle;

        error_handle = GetStdHandle(STD_ERROR_HANDLE);
        WriteFile(error_handle, message, len, &bytes_written, NULL);

        DebugBreak();
        ExitProcess(EXIT_FAILURE);
    }
#endif
}

#include "minic/int.h"
#include "minic/platform.h"
#include "minic/log.h"

#include "win32/windows_base.h"
#include "win32/gdi.h"
#include "win32/misc.h"
#include "win32/sysinfo.h"
#include "win32/winuser.h"

void platform_error(const char *dsc)
{
    WCHAR buf[1024] = L"";
    char msg[1024] = "";

    DWORD format_flags = FORMAT_MESSAGE_FROM_SYSTEM |
                         FORMAT_MESSAGE_IGNORE_INSERTS |
                         FORMAT_MESSAGE_MAX_WIDTH_MASK;

    FormatMessageW(format_flags,
                   NULL,
                   GetLastError() & 0xFFFF,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   buf,
                   sizeof(buf) / sizeof(WCHAR),
                   NULL);

    WideCharToMultiByte(CP_UTF8, 0, buf, -1, msg, sizeof(msg), NULL, NULL);

    log_error("%s: %s", dsc, msg);
}

//static BOOL platform_is_windows10_build_or_greater(WORD build)
//{
//    OSVERSIONINFOEXW osvi = {};
//    osvi.dwOSVersionInfoSize = sizeof(osvi);
//    osvi.dwMajorVersion = 10;
//    osvi.dwMinorVersion = 0;
//    osvi.dwBuildNumber = build;
//
//    // DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;
//
//    ULONGLONG cond = 0;
//
//    cond = VerSetConditionMask(cond, VER_MAJORVERSION, VER_GREATER_EQUAL);
//    cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
//    cond = VerSetConditionMask(cond, VER_BUILDNUMBER, VER_GREATER_EQUAL);
//
//    // HACK: Use RtlVerifyVersionInfo instead of VerifyVersionInfoW as the
//    //       latter lies unless the user knew to embed a non-default manifest
//    //       announcing support for Windows 10 via supportedOS GUID
//
//    return false;
//    // return RtlVerifyVersionInfo()
//}


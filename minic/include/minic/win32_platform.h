#pragma once
#include "minic/assert.h"
#include "minic/log.h"

//#include <Windows.h>
#include "win32/windows_base.h"
#include "win32/gdi.h"
#include "win32/misc.h"
#include "win32/sysinfo.h"
#include "win32/winuser.h"

struct Platform
{
    struct
    {
        struct
        {
            HINSTANCE handle;
        } winmm;

        struct
        {
            HINSTANCE handle;
        } user32;
    } win32;
};

void platform_init(Platform *platform);

static LRESULT CALLBACK window_message_proc(HWND hwnd,
                                            UINT umsg,
                                            WPARAM wparam,
                                            LPARAM lparam)
{
    (void)wparam, (void)lparam;

    Window *window = cast(Window *, GetPropW(hwnd, L"MINIC"));
    if (window == NULL)
    {
        switch (umsg)
        {
            case WM_NCCREATE:

                break;
            default:
                break;
        }
    }

    return 0LL;
}

static void platform_register_window_class(Platform *platform)
{
    (void)platform;

    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = cast(WNDPROC, window_message_proc);
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hCursor = LoadCursorW(NULL, unsafe_cast(LPCWSTR, IDC_ARROW));
    wc.lpszClassName = L"MINIC";

    wc.hIcon = cast(HICON,
                    LoadImageW(wc.hInstance,
                               L"MINIC_ICON",
                               IMAGE_ICON,
                               0,
                               0,
                               LR_DEFAULTSIZE | LR_SHARED));
    if (wc.hIcon == NULL)
    {
        wc.hIcon = cast(HICON,
                        LoadImageW(NULL,
                                   unsafe_cast(LPCWSTR, IDI_APPLICATION),
                                   IMAGE_ICON,
                                   0,
                                   0,
                                   LR_DEFAULTSIZE | LR_SHARED));
    }

    bool registered = RegisterClassExW(&wc);
    assert(registered == true);
}

static BOOL platform_is_windows10_build_or_greater(WORD build)
{
    OSVERSIONINFOEXW osvi = {};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    osvi.dwMajorVersion = 10;
    osvi.dwMinorVersion = 0;
    osvi.dwBuildNumber = build;

    // DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;

    ULONGLONG cond = 0;

    cond = VerSetConditionMask(cond, VER_MAJORVERSION, VER_GREATER_EQUAL);
    cond = VerSetConditionMask(cond, VER_MINORVERSION, VER_GREATER_EQUAL);
    cond = VerSetConditionMask(cond, VER_BUILDNUMBER, VER_GREATER_EQUAL);

    // HACK: Use RtlVerifyVersionInfo instead of VerifyVersionInfoW as the
    //       latter lies unless the user knew to embed a non-default manifest
    //       announcing support for Windows 10 via supportedOS GUID

    return false;
    // return RtlVerifyVersionInfo()
}

void platform_init(Platform *platform)
{
    assert(platform != NULL);

    // platform->win32.user32.handle = LoadLibraryA("User32.dll");
    // assert(platform->win32.user32.handle != NULL);

    platform_register_window_class(platform);

    platform_is_windows10_build_or_greater(0);
}

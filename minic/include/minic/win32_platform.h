#pragma once
#include "minic/assert.h"
#include "minic/log.h"

#include "win32/windows_base.h"
#include "win32/gdi.h"
#include "win32/misc.h"
#include "win32/sysinfo.h"

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
    WNDCLASSEXW wc = {};

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = cast(WNDPROC, window_message_proc);
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hCursor = LoadCursorW(0, IDC_ARROW);
    wc.lpszClassName = L"MINIC";

    wc.hIcon = LoadImageW(wc.hInstance,
                          L"MINIC_ICON",
                          IMAGE_ICON,
                          0,
                          0,
                          LR_DEFAULTSIZE | LR_SHARED);
    if (wc.hIcon == NULL)
    {
        wc.hIcon = LoadImageW(NULL,
                              IDI_APPLICATION,
                              IMAGE_ICON,
                              0,
                              0,
                              LR_DEFAULTSIZE | LR_SHARED)
    }

    bool registered = RegisterClassExW(&wc);
    assert(registered == true);
}

void platform_init(Platform *platform)
{
    assert(platform != NULL);

    platform->win32.user32.handle = LoadLibraryA("User32.lib");
    assert(platform->win32.user32.handle != NULL);

    platform_register_window_class(platform);
}

BOOL platform_is_windows10_build_or_greater(WORD build)
{
    OSVERSIONINFOEXW osvi = { sizeof(osvi), 10, 0, build };

    DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;

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

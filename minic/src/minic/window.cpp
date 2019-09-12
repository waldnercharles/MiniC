#include "minic/window.h"
#include "minic/assert.h"
#include "minic/log.h"

//#include "stddef.h"

//#include <Windows.h>
#include "win32/winuser.h"
#include "win32/misc.h"
#include "win32/gdi.h"

static void platform_error(const char *dsc)
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

void window_init(Window *window, const wchar *title)
{
    assert(window != NULL);
    assert(title != NULL);

    window->title = title;
}

void window_create(Window *window)
{
    assert(window != NULL);
    assert(window->title != NULL);

    HINSTANCE instance = GetModuleHandleW(NULL);

    window->win32.window_handle = CreateWindowExW(0,
                                                  L"MINIC",
                                                  window->title,
                                                  WS_OVERLAPPEDWINDOW,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  NULL,
                                                  NULL,
                                                  instance,
                                                  NULL);
    if (window->win32.window_handle == NULL)
    {
        platform_error("win32: Failed to create window");
        return;
    }
}

void window_show(Window *window)
{
    assert(window != NULL);
    assert(window->win32.window_handle != NULL);

    ShowWindow(window->win32.window_handle, 5);
}

void window_hide(Window *window)
{
    assert(window != NULL);
    assert(window->win32.window_handle != NULL);

    ShowWindow(window->win32.window_handle, 0);
}

void window_maximize(Window *window)
{
    assert(window != NULL);
    assert(window->win32.window_handle != NULL);

    ShowWindow(window->win32.window_handle, 3);
}

void window_minimize(Window *window)
{
    assert(window != NULL);
    assert(window->win32.window_handle != NULL);

    ShowWindow(window->win32.window_handle, 3);
}

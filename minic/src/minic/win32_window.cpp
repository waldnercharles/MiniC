#include "minic/assert.h"
#include "minic/log.h"
#include "minic/platform.h"
#include "minic/window.h"

#include "win32/gdi.h"
#include "win32/misc.h"
#include "win32/winuser.h"

static LRESULT CALLBACK window_message_proc(HWND hwnd,
                                            UINT umsg,
                                            WPARAM wparam,
                                            LPARAM lparam)
{
    switch (umsg)
    {
        case WM_PAINT:
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            break;
    }

    return DefWindowProcW(hwnd, umsg, wparam, lparam);
}

static bool window_register_window_class()
{
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

    if (registered)
    {
        log_debug("Win32: Window class registration successful.");
    }
    else
    {
        log_error("Win32: Window class registration failed.");
    }

    return registered;
}

void window_init(Window *window,
                 const wchar *title,
                 s32 width,
                 s32 height,
                 s32 xpos,
                 s32 ypos)
{
    assert(window != NULL);
    assert(title != NULL);

    static bool registered = false;
    if (!registered && !(registered = window_register_window_class()))
    {
        return;
    }

    window->title = title;

    HINSTANCE instance = GetModuleHandleW(NULL);

    window->win32.window_handle = CreateWindowExW(0,
                                                  L"MINIC",
                                                  window->title,
                                                  WS_OVERLAPPEDWINDOW,
                                                  xpos,
                                                  ypos,
                                                  width,
                                                  height,
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

void window_init(Window *window, const wchar *title, s32 width, s32 height)
{
    window_init(window, title, width, height, CW_USEDEFAULT, CW_USEDEFAULT);
}

void window_init(Window *window, const wchar *title)
{
    window_init(window,
                title,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT);
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

#include "minic/window.h"
#include "minic/assert.h"

#include "win32/winuser.h"
#include "win32/misc.h"
#include "win32/gdi.h"

void window_init(Window *window, const char *title)
{
    assert(window != NULL);
    assert(title != NULL);

    window->title = title;
}

void window_create(Window *window)
{
    assert(window != NULL);
    assert(window->title != NULL);

    window->win32.window_handle = CreateWindowExA(0L,
                                                  "MINIC",
                                                  window->title,
                                                  WS_OVERLAPPEDWINDOW,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  CW_USEDEFAULT,
                                                  NULL,
                                                  NULL,
                                                  GetModuleHandleA(NULL),
                                                  NULL);
    if (window->win32.window_handle == NULL)
    {
        DWORD err = GetLastError();
        (void)err;
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

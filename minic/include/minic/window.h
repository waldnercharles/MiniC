#pragma once

#include "minic/int.h"

#include "win32/windows_base.h"
#include "win32/window.h"

struct Monitor
{
};

struct Win32Window
{
    HWND window_handle;
    WNDCLASS window_class;
};

struct Window
{
    const wchar *title;

    Win32Window win32;
};

void window_init(Window *window,
                 const wchar *title,
                 s32 width,
                 s32 height,
                 s32 xpos,
                 s32 ypos);

void window_init(Window *window, const wchar *title, s32 width, s32 height);
void window_init(Window *window, const wchar *title);

void window_show(Window *window);
void window_hide(Window *window);

void window_maximize(Window *window);
void window_minimize(Window *window);

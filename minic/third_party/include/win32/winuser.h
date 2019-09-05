#ifndef _WINDOWS_
#ifndef WINDOWS_WINUSER_H
#define WINDOWS_WINUSER_H

/* Disable all warnings */
#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#ifndef WINDOWS_BASE_H
#include "windows_base.h"
#endif

#ifndef WINDOWS_WINDOW_H
#include "window.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
    UINT cbSize;

    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCSTR lpszMenuName;
    LPCSTR lpszClassName;

    HICON hIconSm;
} WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;

typedef struct
{
    UINT cbSize;

    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    LPCWSTR lpszMenuName;
    LPCWSTR lpszClassName;

    HICON hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;

ATOM WINAPI RegisterClassExA(const WNDCLASSEXA *);
ATOM WINAPI RegisterClassExW(const WNDCLASSEXW *);

BOOL WINAPI SetPropA(HWND hWnd, LPCSTR lpString, HANDLE hData);
BOOL WINAPI SetPropW(HWND hWnd, LPCWSTR lpString, HANDLE hData);

HANDLE WINAPI GetPropA(HWND hWnd, LPCSTR lpString);
HANDLE WINAPI GetPropW(HWND hWnd, LPCWSTR lpString);

HANDLE WINAPI RemovePropA(HWND hWnd, LPCSTR lpString);
HANDLE WINAPI RemovePropW(HWND hWnd, LPCWSTR lpString);

#define IMAGE_BITMAP 0
#define IMAGE_ICON 1
#define IMAGE_CURSOR 2
#define IMAGE_ENHMETAFILE 3

#define LR_DEFAULTCOLOR 0x00000000
#define LR_MONOCHROME 0x00000001
#define LR_COLOR 0x00000002
#define LR_COPYRETURNORG 0x00000004
#define LR_COPYDELETEORG 0x00000008
#define LR_LOADFROMFILE 0x00000010
#define LR_LOADTRANSPARENT 0x00000020
#define LR_DEFAULTSIZE 0x00000040
#define LR_VGACOLOR 0x00000080
#define LR_LOADMAP3DCOLORS 0x00001000
#define LR_CREATEDIBSECTION 0x00002000
#define LR_COPYFROMRESOURCE 0x00004000
#define LR_SHARED 0x00008000

#define WM_NCCREATE 0x0081

#define WS_EX_APPWINDOW 0x00040000L

HANDLE WINAPI LoadImageA(HINSTANCE hInst,
                         LPCSTR name,
                         UINT type,
                         int cx,
                         int cy,
                         UINT fuLoad);

HANDLE WINAPI LoadImageW(HINSTANCE hInst,
                         LPCWSTR name,
                         UINT type,
                         int cx,
                         int cy,
                         UINT fuLoad);
#if defined(__cplusplus)
}
#endif

/* Enable all warnings */
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif /* WINDOWS_WINUSER_H */
#endif /* _WINDOWS_ */
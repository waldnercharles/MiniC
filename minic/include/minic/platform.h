#pragma once

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
void platform_error(const char *dsc);

void platform_init(Platform *platform)
{
    assert(platform != NULL);

    // platform->win32.user32.handle = LoadLibraryA("User32.dll");
    // assert(platform->win32.user32.handle != NULL);
}

#define DEBUG

//#define __STDDEF_H
//#define NULL nullptr

#define UNICODE
//#define NOMINMAX
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include "minic/assert.h"

//#include "minic/iterator.cpp"

//#include "minic/asset.cpp"

#include "minic/allocator.cpp"
#include "minic/array.cpp"
#include "minic/fp.cpp"
//#include "minic/hashtable.c"
#include "minic/handle.cpp"
#include "minic/io.cpp"
#include "minic/math.cpp"
#include "minic/memory.cpp"
#include "minic/rand.cpp"
#include "minic/string.cpp"
#include "minic/timer.cpp"

#include "minic/win32_allocator_default.cpp"
#include "minic/win32_assert.cpp"
#include "minic/win32_file.cpp"
#include "minic/win32_io.cpp"
#include "minic/win32_log.cpp"
#include "minic/win32_timer.cpp"

#include "minic/win32_platform.cpp"
#include "minic/win32_window.cpp"

extern "C" void *__stdcall memset(void *dst, int val, size_t size);
extern "C" int __stdcall mainCRTStartup();

#include "main.cpp"

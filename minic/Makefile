
VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community
VS_VERSION=14.15.26726

VC_PATH=$(VS_PATH)\VC\Tools\MSVC\$(VS_VERSION)

WINSDK_PATH=C:\Program Files (x86)\Windows Kits\10
WINSDK_VERSION=10.0.16299.0

CC=clang-cl

SYSTEM_INCLUDES = \
	-imsvc"$(VC_PATH)\include" \
	-imsvc"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\shared" \
	-imsvc"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\ucrt" \
	-imsvc"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\um"

INCLUDES = -Isrc -Iinclude -imsvcthird_party\include
IGNORED_WARNINGS = -wd4201 -wd4189 -D_CRT_SECURE_NO_WARNINGS

CFLAGS = \
	-analyze- \
	-nologo \
	-Od -Oi -GR- -GS- -EHa- -Zi -Zo -Zc:inline -fp:fast -fp:except- \
	-Wall -Wextra -Wpedantic -Weverything -Werror \
	-Wno-language-extension-token -Wno-gnu-zero-variadic-macro-arguments \
	-ferror-limit=256 \
	$(INCLUDES) $(IGNORED_WARNINGS)

LFLAGS = \
	-machine:x64 \
	-nodefaultlib \
	-subsystem:console \
	-opt:ref \
	-incremental:no \
	-libpath:"$(VC_PATH)\lib\x64" \
	-libpath:"$(WINSDK_PATH)\lib\$(WINSDK_VERSION)\ucrt\x64" \
	-libpath:"$(WINSDK_PATH)\lib\$(WINSDK_VERSION)\um\x64" \
	-libpath:"third_party\lib\x64" \
	libucrt.lib libvcruntime.lib libcmt.lib libcpmt.lib \
	legacy_stdio_definitions.lib oldnames.lib \
	legacy_stdio_wide_specifiers.lib \
	kernel32.lib User32.lib

main: src\unity_build.c
	$(CC) $(CFLAGS) src\unity_build.c -Fobin\unity_build.obj -Fdbin\main.pdb -Febin\main.exe -link $(LFLAGS)
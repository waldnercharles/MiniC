
VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community
VS_VERSION=14.22.27905

VC_PATH=$(VS_PATH)\VC\Tools\MSVC\$(VS_VERSION)

WINSDK_PATH=C:\Program Files (x86)\Windows Kits\10
WINSDK_VERSION=10.0.18362.0

CC=clang-cl

SYSTEM_INCLUDES = \
	-isystem"$(VC_PATH)\include" \
	-isystem"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\shared" \
	-stdlib++-isystem"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\ucrt" \
	-isystem"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\um"

INCLUDES = -Isrc -Iinclude -imsvcthird_party\include
IGNORED_WARNINGS = -wd4201 -wd4189 -D_CRT_SECURE_NO_WARNINGS

CFLAGS = \
	-analyze- \
	-nologo \
	-Od -Oi -GR- -GS- -EHa- -Zi -Zo -Zc:inline -fp:fast -fp:except- \
	-Wall -Wextra -Wpedantic -Weverything -Werror -Wuninitialized \
	-Wno-language-extension-token -Wno-gnu-zero-variadic-macro-arguments -Wno-c++98-compat -Wno-c++98-compat-pedantic \
	-Wno-shadow-field-in-constructor -Wno-shadow \
	-Wno-gnu-anonymous-struct -Wno-assume -Wno-reserved-id-macro \
	-ferror-limit=256 \
	-nobuiltininc \
	-imsvc"$(VC_PATH)\include" \
	-imsvc"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\shared" \
	-imsvc"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\ucrt" \
	-imsvc"$(WINSDK_PATH)\Include\$(WINSDK_VERSION)\um" \
	$(INCLUDES) $(IGNORED_WARNINGS) \
	-Xclang -std=c++17

LFLAGS = \
	-machine:x64 \
	-nodefaultlib \
	-subsystem:console \
	-opt:ref \
	-incremental:no \
	-libpath:"$(VC_PATH)\lib\x64" \
	-libpath:"$(WINSDK_PATH)\lib\$(WINSDK_VERSION)\um\x64" \
	-libpath:"$(WINSDK_PATH)\lib\$(WINSDK_VERSION)\ucrt\x64" \
	user32.lib gdi32.lib winmm.lib kernel32.lib opengl32.lib

	#-libpath:"third_party\lib\x64"

	# libucrt.lib libvcruntime.lib libcmt.lib libcpmt.lib
	# legacy_stdio_definitions.lib oldnames.lib
	# legacy_stdio_wide_specifiers.lib
	# kernel32.lib User32.lib

main: src\unity_build.cpp
	$(CC) $(CFLAGS) src\unity_build.cpp -Fobin\unity_build.obj -Fdbin\main.pdb -Febin\main.exe -link $(LFLAGS)
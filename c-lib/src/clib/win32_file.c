#include <clib/int.h>
#include <win32/file.h>

#define FILE_MAX_PATH 1024
#define FILE_MAX_FILENAME 256
#define FILE_MAX_EXTENSION 32

typedef struct
{
    char path[FILE_MAX_PATH];
    bool has_next;
    HANDLE handle;
    WIN32_FIND_DATAA fdata;
} Directory;

typedef struct
{
    char path[FILE_MAX_PATH];
    char name[FILE_MAX_FILENAME];
    char extension[FILE_MAX_EXTENSION];
    bool is_directory;
    bool is_reg;  // ?
    size_t size;
} File;

typedef struct
{
    FILETIME time;
} Time;

void
directory_open(Directory *directory, char *path)
{
}
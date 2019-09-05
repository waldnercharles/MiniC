#include "minic/assert.h"
#include "minic/file.h"
#include "minic/int.h"
#include "minic/log.h"
#include "minic/string.h"

//#include <Windows.h>
#include <win32/file.h>
#include <win32/io.h>
#include <win32/misc.h>

struct Directory
{
    char path[FILE_MAX_PATH];
    bool has_next;
    HANDLE handle;
    WIN32_FIND_DATAA file;
};

struct File
{
    char path[FILE_MAX_PATH];
    char name[FILE_MAX_FILENAME];
    char ext[FILE_MAX_EXTENSION];
    bool is_dir;
    bool is_reg;
    usize size;
};

static void file_read_ext(File *file)
{
    assert(file != NULL);

    char *filename = file->name;
    char *ext = NULL;

    while (*filename++)
    {
        if (*filename == '.')
        {
            ext = filename;
        }
    }

    if (ext != NULL)
    {
        string_copy(file->ext, ext, FILE_MAX_EXTENSION);
    }
    else
    {
        *file->ext = 0;

        if (!file->is_dir)
        {
            log_warning("No extension found: %s", file->name);
        }
    }
}

inline bool directory_has_next(Directory *dir)
{
    return dir->has_next;
}

inline const char *directory_path(Directory *dir)
{
    return dir->path;
}

bool directory_open(Directory *dir, const char *path)
{
    assert(dir != NULL);
    assert(path != NULL);
    usize n;

    n = string_copy(dir->path, path, FILE_MAX_PATH) - 1ull;
    n += string_copy(dir->path + n, "\\*", FILE_MAX_PATH - n);

    dir->handle = FindFirstFileA(dir->path, &(dir->file));

    assert(n >= 3);
    dir->path[n - 3] = 0;

    if (dir->handle == INVALID_HANDLE_VALUE)
    {
        // TODO: Error codes
        log_error("Failed to open directory (%s)", path);
        directory_close(dir);

        assert(dir->handle != INVALID_HANDLE_VALUE);
        return false;
    }

    dir->has_next = true;
    log_info("Directory Opened: %s", dir->path);

    return dir->has_next;
}

void directory_close(Directory *dir)
{
    dir->path[0] = 0;
    dir->has_next = false;
    if (dir->handle != INVALID_HANDLE_VALUE)
    {
        FindClose(dir->handle);
    }
}

void directory_next(Directory *dir)
{
    assert(dir != NULL);
    assert(dir->has_next);

    DWORD err = ERROR_SUCCESS;
    if (!FindNextFileA(dir->handle, &dir->file))
    {
        dir->has_next = false;
        err = GetLastError();
        assert(err == ERROR_SUCCESS || err == ERROR_NO_MORE_FILES);
    }

    if (err == ERROR_SUCCESS)
    {
        log_info("Found file: %s", dir->file.cFileName);
    }
    if (!dir->has_next)
    {
        log_info("No more files");
    }
}

void directory_read_file(Directory *dir, File *file)
{
    assert(dir != NULL);
    assert(dir->handle != INVALID_HANDLE_VALUE);

    usize n = 0;

    const char *dpath = dir->path;
    char *fpath = file->path;

    n = string_copy(fpath, dpath, FILE_MAX_PATH) - 1;
    n += string_copy(fpath + n, "/", FILE_MAX_PATH - n) - 1;

    const char *dname = dir->file.cFileName;
    char *fname = file->name;

    string_copy(fname, dname, FILE_MAX_FILENAME);
    string_copy(fpath + n, fname, FILE_MAX_PATH - n);

    file->size =
        ((cast(usize, dir->file.nFileSizeHigh) * (cast(usize, U32_MAX) + 1)) +
         dir->file.nFileSizeLow);

    file->is_dir = !!(dir->file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    file->is_reg = !!(dir->file.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) ||
                   !(dir->file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

    file_read_ext(file);
}

inline const char *file_name(File *file)
{
    return file->name;
}

inline const char *file_ext(File *file)
{
    return file->ext;
}

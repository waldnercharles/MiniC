#pragma once

#include "minic/int.h"

#define FILE_MAX_PATH 1024ull
#define FILE_MAX_FILENAME 256ull
#define FILE_MAX_EXTENSION 32ull

typedef struct Directory Directory;
typedef struct File File;

inline bool
directory_has_next(Directory *dir);

inline const char *
directory_path(Directory *dir);

bool
directory_open(Directory *dir, const char *path);

void
directory_close(Directory *dir);

void
directory_next(Directory *dir);

void
directory_read_file(Directory *dir, File *file);

inline const char *
file_name(File *file);

inline const char *
file_ext(File *file);

#ifndef MINIC_FILE_H
#define MINIC_FILE_H

#include "minic/int.h"

#define FILE_MAX_PATH 1024ull
#define FILE_MAX_FILENAME 256ull
#define FILE_MAX_EXTENSION 32ull

typedef struct Directory Directory;
typedef struct File File;

inline bool
directory_has_next(Directory *dir);

inline char *
directory_path(Directory *dir);

bool
directory_open(Directory *dir, char *path);

void
directory_close(Directory *dir);

void
directory_next(Directory *dir);

void
directory_read_file(Directory *dir, File *file);

void
file_read_ext(File *file);

#endif

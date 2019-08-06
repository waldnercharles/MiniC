#pragma once

#include "minic/allocator.h"

typedef struct VirtualFileSystem VirtualFileSystem;

void
vfs_init(VirtualFileSystem *loader, Allocator *allocator);

void
vfs_mount(VirtualFileSystem *loader, char *path, char *mount);

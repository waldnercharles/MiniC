#include "minic/allocator.h"
#include "minic/assert.h"
#include "minic/asset.h"

#include "minic/allocator_default.h"
#include "minic/file.h"
#include "minic/string.h"

struct VirtualFileSystem
{
    Allocator *allocator;
};

// void
// asset_test()
//{
//    Allocator allocator;
//    allocator_init_default(&allocator);
//
//    AssetLoader loader;
//    asset_loader_init(&loader, &allocator);
//}

void
vfs_init(VirtualFileSystem *vfs, Allocator *allocator)
{
    assert(vfs != NULL);
    assert(allocator != NULL);

    vfs->allocator = allocator;

    // TODO: Strpool
}

#define Array(type) type *

void
vfs_mount(VirtualFileSystem *loader, char *path, char *mount)
{
    assert(loader != NULL);

    assert(path != NULL);
    assert(mount != NULL);

    usize path_length = string_length(path);
    assert(path_length == 0 || path[0] != '/');
    assert(path_length == 0 || path[path_length - 1] != '/');

    usize mount_length = string_length(mount);
    assert(mount_length == 0 || mount[0] != '/');
    assert(mount_length == 0 || mount[mount_length - 1] != '/');

    bool is_valid = (string_find_char(path, '\\') == NULL) &&
                    (string_find_char(mount, '\\') == NULL) &&
                    (string_find_char(mount, ':') == NULL);
    assert(is_valid);

    // TODO: Handle zip files

    Directory dir;
    if (directory_open(&dir, *path == '\0' ? "." : path))
    {
    }
}

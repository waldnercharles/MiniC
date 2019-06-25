#include "minic/allocator.h"
#include "minic/assert.h"
#include "minic/asset.h"

#include "minic/allocator_default.h"
#include "minic/file.h"
#include "minic/string.h"

struct AssetLoader
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
asset_loader_init(AssetLoader *loader, Allocator *allocator)
{
    assert(loader != NULL);
    assert(allocator != NULL);

    loader->allocator = allocator;

    // TODO: Strpool
}

void
asset_loader_mount(AssetLoader *loader, char *path, char *mount)
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

    Directory dir;
    directory_open(&dir, *path == '\0' ? "." : path);
}

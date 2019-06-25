#pragma once

#include "minic/allocator.h"

typedef struct AssetLoader AssetLoader;

void
asset_loader_init(AssetLoader *loader, Allocator *allocator);

void
asset_loader_mount(AssetLoader *loader, char *path, char *mount);

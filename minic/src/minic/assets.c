#include "minic/allocator.h"

typedef struct StringPool StringPool;

typedef union Handle Handle;

typedef struct StringPoolConfig
{
} StringPoolConfig;

union Handle
{
    struct
    {
        u32 index;
        u32 generation;
    };
    u64 raw;
};

struct StringPool
{
    Allocator *allocator;
    bool ignorecase;
    u32 counterbits;
    u32 indexbits;
    u32 entrycapacity;
    u32 blockcapacity;
};

void
strpool_init(StringPool *pool, Allocator *allocator)
{
    pool->allocator = allocator;
    pool->ignorecase = false;

    pool->counterbits = 32;
    pool->indexbits = 32;

    pool->entrycapacity = 4096;
    pool->blockcapacity = 32;
    pool->blocksize = 256 * 1024;
    pool->minlength = 23 * sizeof(char);
}

typedef struct
{
} AssetLoader;
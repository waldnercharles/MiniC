#include "minic/allocator_default.h"
#include "minic/array.h"
#include "minic/int.h"
#include "minic/io.h"
//#include "minic/asset.h"
//#include "minic/file.h"
//#include "minic/handle.h"
//#include "minic/hashtable.h"
//#include "minic/int.h"
//#include "minic/io.h"
//#include "minic/iterator.h"
//#include "minic/rand.h"
//
//#include "minic/generic_array.h"

struct Banana
{
    int x;
    int y;
    int z;
};

// s32
// main(void)
//{
//    assetsys_t *assetsys = assetsys_create(0);
//
//    // Mount current working folder as a virtual "/data" path
//    assetsys_mount(assetsys, ".", "/data");
//    // Print all files and subfolders
//    list_assets(assetsys, "/", 0);  // Start at root
//    // Load a file
//    assetsys_file_t file;
//    assetsys_file(assetsys, "/data/readme.txt", &file);
//    int size = assetsys_file_size(assetsys, file);
//    char *content = (char *)malloc(size + 1);  // extra space for '\0'
//    int loaded_size = 0;
//    assetsys_file_load(assetsys, file, &loaded_size, content, size);
//    content[size] = '\0';  // zero terminate the text file
//    printf("%s\n", content);
//    free(content);
//    assetsys_destroy(assetsys);
//}

// array_define(s8, Array_s8);
// array_define(u8, Array_u8);
// array_define(s16, Array_s16);
// array_define(u16, Array_u16);
// array_define(s32, Array_s32);
// array_define(u32, Array_u32);
// array_define(s64, Array_s64);
// array_define(u64, Array_u64);
// array_define(void *, Array);

s32 main()
{
    CAllocator allocator;
    Array<Banana> arr(&allocator);

    Banana banana;
    banana.x = 7;
    banana.y = 4;
    banana.z = 2;

    arr.push_back(banana);

    // const Banana b = { 7, 0, 0 };

    // arr.push_back(b);
    // arr.push_back(rvalue(b));

    // io_printf("%i\n", arr.capacity);
    io_printf("%i\n", arr.front().x);

    // io_printf("Hello World! %i", *my_int);

    /*allocator_init_default(&allocator);

    Directory directory;
    directory_open(&directory, ".");

    while (directory_has_next(&directory))
    {
        File file;
        directory_read_file(&directory, &file);

        io_printf("%s\n", file_name(&file));

        directory_next(&directory);
    }

    directory_close(&directory);

    VirtualFileSystem asset_loader;
    vfs_init(&asset_loader, &allocator);*/

    /*HandleManager handle_manager;
    handle_init(&handle_manager, &allocator, 1000);

    Array arr;
    array_init(&arr, &allocator, sizeof(u32));

    for (u32 i = 0; i < 10; i++)
    {
        u32 handle_index = handle_create(&handle_manager, i);
        array_push_back(&arr, &handle_index);
    }

    for (u32 i = 0; i < 5; i++)
    {
        u32 handle_index = *cast(u32 *, array_get(&arr, 0));
        array_remove(&arr, 0);

        handle_dispose(&handle_manager, handle_index);

        handle_index = handle_create(&handle_manager, i);
        array_push_back(&arr, &handle_index);
    }

    for (u32 i = 0; i < array_count(&arr); i++)
    {
        u32 handle_index = *cast(u32 *, array_get(&arr, i));
        io_printf("%u:(%u, %u) ",
                  handle_index,
                  handle_key(&handle_manager, handle_index),
                  handle_generation(&handle_manager, handle_index));
    }*/

    return 0;
}

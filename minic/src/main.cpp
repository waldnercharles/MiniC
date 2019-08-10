#include "minic/allocator_default.h"
#include "minic/array.h"
#include "minic/handle.h"
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

    HandleManager handle_manager(&allocator);
    Array<Handle> handles(&allocator);
    for (int i = 0; i < 1000; i++)
    {
        handles.push_back(handle_manager.create());

        io_printf("(%i, %i)\n",
                  handles[i].get_index(),
                  handles[i].get_generation());
    }

    return 0;
}

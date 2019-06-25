#include "minic/allocator_default.h"
#include "minic/array.h"
#include "minic/file.h"
#include "minic/handle.h"
#include "minic/hashtable.h"
#include "minic/int.h"
#include "minic/io.h"
#include "minic/iterator.h"
#include "minic/rand.h"

s32
main()
{
    Allocator allocator;
    allocator_init_default(&allocator);

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

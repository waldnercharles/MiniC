#include "minic/int.h"
#include "minic/io.h"
#include "minic/rand.h"

s32
main()
{
    Random rand;
    u64 seed = 0x1726443095127773;
    rand_seed(&rand, seed);

    for (s32 i = 0; i < 10; i++)
    {
        io_printf("Hello World! %llu\n", rand_next_u64(&rand));
    }

    return 0;
}

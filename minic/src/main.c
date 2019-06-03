#include "minic/int.h"
#include "minic/io.h"
#include "minic/file.h"
//#include "minic/log.h"
//#include "minic/rand.h"
//#include "minic/string.h"
//#include "minic/timer.h"

s32
main()
{
    Directory dir;
    File file;

    directory_open(&dir, "..");
    directory_read_file(&dir, &file);

    return 0;
}

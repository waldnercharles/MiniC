#ifndef TEST_H
#define TEST_H

#include <time.h>

#include "clib/int.h"
#include "clib/io.h"

#ifdef _WIN32
#include <win32/io.h>
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define access _access
#define close _close
#define dup _dup
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

char *const null_filename = "NUL";
#else
#include <unistd.h>
char *const null_filename = "/dev/null";
#endif

static u32 num_tests = 0;
static u32 num_fails = 0;

#define test_results()                                                         \
    do                                                                         \
    {                                                                          \
        __printf("%d of %d tests passed (%d failed)\n",                          \
               num_tests - num_fails,                                          \
               num_tests,                                                      \
               num_fails);                                                     \
    } while (0)

#define test_run(test_func)                                                    \
    do                                                                         \
    {                                                                          \
        const u32 num_tests_start = num_tests;                                 \
        const u32 num_fails_start = num_fails;                                 \
        const clock_t start = clock();                                         \
        const char *passed_status = "PASS";                                    \
        const char *failed_status = "FAIL";                                    \
        s32 stdout_backup;                                                     \
        FILE *null_out;                                                        \
        __printf("Running %s -- \n", #test_func);                                \
                                                                               \
        stdout_backup = dup(STDOUT_FILENO);                                    \
        fflush(stdout);                                                        \
                                                                               \
        null_out = fopen(null_filename, "w");                                  \
        dup2(fileno(null_out), STDOUT_FILENO);                                 \
                                                                               \
        test_func();                                                           \
                                                                               \
        fflush(stdout);                                                        \
        fclose(null_out);                                                      \
                                                                               \
        dup2(stdout_backup, STDOUT_FILENO);                                    \
        close(stdout_backup);                                                  \
                                                                               \
        __printf("\t[ %s ] [ %dms ] -- %s\n",                                    \
               num_fails > num_fails_start ? failed_status : passed_status,    \
               (int)((clock() - start) * 1000 / CLOCKS_PER_SEC),               \
               #test_func);                                                    \
    } while (0)

#define test_assert(x)                                                         \
    do                                                                         \
    {                                                                          \
        ++num_tests;                                                           \
        if (!(x))                                                              \
        {                                                                      \
            ++num_fails;                                                       \
            printf("\t[ ASSERTION FAILED ] -- (%s)\n", #x);                    \
        }                                                                      \
    } while (0)

#endif
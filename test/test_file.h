#ifndef CLIB_TEST_FILE_H
#define CLIB_TEST_FILE_H

#include <clib/unittest.h>
#include <clib/file.h>
#include <clib/log.h>

static inline void c_test_file(CUnit_Test* unit_test)
{
    c_test_register(unit_test, c_test_list_readline);
}

void c_test_list_readline(CUnit_Test* self)
{
    char* gt[] = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n",
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n",
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n",
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n"
    };
    CFile file1 = c_file_open(test_path "/input/file1", "r");

    cstr buf = c_string_new(150);
    u32 chars_len = 0;
    int iii = 0;
    while((chars_len = c_file_readline(&file1, buf)) > 0)
    {
        printf("Chars: %d\n%s\n", chars_len, buf);
        TEST_CHECK(c_string_equal(gt[iii], buf));
        iii++;
    }

    c_file_close(&file1);
}

#else
#error "This unittests must be called once"
#endif // CLIB_TEST_FILE_H
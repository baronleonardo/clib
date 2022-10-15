#ifndef CLIB_TEST_FILE_H
#define CLIB_TEST_FILE_H

#include <clib/unittest.h>
#include <clib/file.h>
#include <clib/log.h>
#include <clib/list.h>

static inline void c_test_file(CUnit_Test* unit_test)
{
    c_test_register(unit_test, c_test_list_readline);
    c_test_register(unit_test, c_test_list_read);
    c_test_register(unit_test, c_test_list_write);
}

void c_test_list_readline(CUnit_Test* self)
{
    char* gt[] = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.",
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.",
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
    };
    CFile file1 = c_file_open(test_path "/input/file1", "r");

    cstr buf = c_string_new(150);
    u32 chars_len = 0;
    int iii = 0;
    while((chars_len = c_file_readline(&file1, buf)) > 0)
    {
        // printf("Chars: %d\n%s\n", chars_len, buf);
        c_string_rtrim(buf, "\n", 1);
        TEST_CHECK(c_string_equal(gt[iii], buf));
        iii++;
    }

    c_file_close(&file1);
}

void c_test_list_read(CUnit_Test* self)
{
    // read string file
    CFile file1 = c_file_open(test_path "/input/file1", "r");
    cstr buf = c_string_new(600);
    u32 chars_num = c_file_read(&file1, buf, c_string_capacity(buf), true);
    // char buf[600];
    // u32 chars_num = c_file_read(&file1, buf, 600);
    c_string_update_len(buf);
    char* gt1 = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n"
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n"
    };

    TEST_CHECK(c_string_equal(buf, gt1));

    c_file_close(&file1);

    /*************************************************************/

    // read integers file
    CFile file2 = c_file_open(test_path "/input/file2", "r");

    c_list(int) list = c_list_new_with_capacity(int, 10);
    u32 chars_num2 = c_file_read(&file2, list, c_list_capacity(list), false);
    u32 gt2[] = { 1, 2, 3, 4 };

    for(int iii = 0; iii < chars_num2; ++iii)
    {
        TEST_CHECK(list[iii] = gt2[iii]);
    }

    c_list_free(list);
    c_file_close(&file2);
}

void c_test_list_write(CUnit_Test* self)
{
    CFile file2 = c_file_open(test_path "/output/file2", "w");

    int arr[] = { 1, 2, 3, 4 };
    u32 arr_len = sizeof(arr) / sizeof(arr[0]);

    c_file_write(&file2, arr, arr_len);

    c_file_close(&file2);
}

#else
#error "This unittests must be called once"
#endif // CLIB_TEST_FILE_H
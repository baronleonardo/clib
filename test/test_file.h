#ifndef CLIB_TEST_FILE_H
#define CLIB_TEST_FILE_H

#include <clib/unittest.h>
#include <clib/file.h>
#include <clib/log.h>
#include <clib/list.h>

#include <string.h>

static inline void c_test_file(CUnit_Test* unit_test)
{
    c_test_register(unit_test, c_test_file_write);
    c_test_register(unit_test, c_test_file_readline);
    c_test_register(unit_test, c_test_file_read);
    c_test_register(unit_test, c_test_file_read_write_arabic_names);
}

void c_test_file_readline(CUnit_Test* self)
{
    char* gt[] = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.",
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.",
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.",
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
    };
    CError err;
    char* path_c = test_path "/input/file1";
    cstr path = c_string_new_from_buf(path_c, strlen(path_c));
    CFile file1 = c_file_open(path, "r", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    cstr buf = c_string_new(150);
    u32 chars_len = 0;
    int iii = 0;
    while((chars_len = c_file_readline(&file1, buf, &err)) > 0)
    {
        TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
        // printf("Chars: %d\n%s\n", chars_len, buf);
        c_string_rtrim(buf, "\n", 1, &err);
        TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

        TEST_CHECK(c_string_equal(gt[iii], buf));
        iii++;
    }

    c_file_close(&file1, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
}

void c_test_file_write(CUnit_Test* self)
{
    CError err;

    CFile file2 = c_file_open(test_path "/output/file2", "w", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    int arr[] = { 1, 2, 3, 4 };
    u32 arr_len = sizeof(arr) / sizeof(arr[0]);

    c_file_write(&file2, arr, arr_len, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    c_file_close(&file2, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    /*************************************************************/

    // test arabic
    CFile file_arabic = c_file_open(test_path "/output/file_arabic", "w", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    const char* buf = "اللّه نور السماوات و الأرض";
    cstr str = c_string_new_from_buf(buf, strlen(buf));
    c_file_write(&file_arabic, str, c_string_capacity(str), &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    c_file_close(&file_arabic, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
}

void c_test_file_read(CUnit_Test* self)
{
    CError err;

    // read string file
    CFile file1 = c_file_open(test_path "/input/file1", "r", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
    cstr buf = c_string_new(600);
    u32 chars_num = c_file_read(&file1, buf, c_string_capacity(buf), true, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
    // char buf[600];
    // u32 chars_num = c_file_read(&file1, buf, 600);
    c_string_update_len(buf, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    char* gt1 = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n"
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n"
    };

    TEST_CHECK(c_string_equal(buf, gt1));

    c_file_close(&file1, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    /*************************************************************/

    // read integers file
    CFile file2 = c_file_open(test_path "/input/file2", "r", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    c_list(int) list = c_list_new_with_capacity(int, 10);
    u32 chars_num2 = c_file_read(&file2, list, c_list_capacity(list), false, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    u32 gt2[] = { 1, 2, 3, 4 };

    for(int iii = 0; iii < chars_num2; ++iii)
    {
        TEST_CHECK(list[iii] = gt2[iii]);
    }

    c_list_free(list);
    c_file_close(&file2, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    /*************************************************************/

    // test arabic
    CFile file_arabic = c_file_open(test_path "/output/file_arabic", "r", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    const char* gt_str = "اللّه نور السماوات و الأرض";
    cstr gt = c_string_new_from_buf(gt_str, strlen(gt_str));

    cstr str = c_string_new(100);

    c_file_read(&file_arabic, str, c_string_capacity(str), true, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    TEST_CHECK(c_string_equal(str, gt));

    c_file_close(&file_arabic, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
}

void c_test_file_read_write_arabic_names(CUnit_Test* self)
{
    CError err;

    // test read
    CFile file_arabic = c_file_open(test_path "/input/فايل 3", "r", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    const char* buf = "اللّه نور السماوات و الأرض";
    cstr gt = c_string_new_from_buf(buf, strlen(buf));

    cstr str =  c_string_new(100);
    c_file_read(&file_arabic, str, c_string_capacity(str), true, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    TEST_CHECK(c_string_equal(str, gt));

    c_file_close(&file_arabic, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    /***********************************************************/

    // test write
    CFile file_arabic_out = c_file_open(test_path "/output/فايل_عربي", "w", &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    c_file_write(&file_arabic_out, gt, c_string_len(gt), &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);

    c_file_close(&file_arabic_out, &err);
    TEST_REQUIRE_MESSAGE(err.code == C_NO_ERROR, err.msg);
}

#else
#error "This unittests must be called once"
#endif // CLIB_TEST_FILE_H
#ifndef CLIB_TEST_LIST_H
#define CLIB_TEST_LIST_H

#include <clib/unittest.h>
#include <clib/list.h>
#include <clib/log.h>
#include <math.h>

static inline void c_test_list(CUnit_Test* unit_test)
{
    c_test_register(unit_test, c_test_list_new);
    c_test_register(unit_test, c_test_list_new_from_array);
}

void c_test_list_new(CUnit_Test* self)
{
    int* list = c_list_new(int);
    c_list_push(list, 1);
    c_list_push(list, 2);
    c_list_push(list, 3);
    c_list_push(list, 4);
    c_list_push(list, 5);

    TEST_CHECK(list[0] == 1);
    TEST_CHECK(list[1] == 2);
    TEST_CHECK(list[2] == 3);
    TEST_CHECK(list[3] == 4);
    TEST_CHECK(list[4] == 5);

    c_list_free(list);
}

void c_test_list_new_from_array(CUnit_Test* self)
{
    float arr[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
    u32 arr_size = sizeof(arr) / sizeof(arr[0]);
    const float epsilon = 0.0001;

    float* list = c_list_new_from_array(float, arr, arr_size);

    for(int iii = 0; iii < c_list_len(list); iii++)
    {
        TEST_CHECK(abs(list[iii] - arr[iii]) < epsilon);
    }

    c_list_free(list);
}

#else
#error "This unittests must be called once"
#endif // CLIB_TEST_LIST_H
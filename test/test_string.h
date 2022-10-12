#ifndef CLIB_TEST_STRING_H
#define CLIB_TEST_STRING_H

#include <clib/unittest.h>
#include <clib/string.h>
#include <clib/log.h>

static inline void c_test_string(CUnit_Test* unit_test)
{
    c_test_register(unit_test, c_test_string_new);
}

void c_test_string_new(CUnit_Test* self)
{
    // char* str_buf = "My name is CLib";
    char* str_buf = "ماي نيم ايز خان";
    u32 str_buf_size = strlen(str_buf);
    cstr str = c_string_new_from_buf(str_buf, str_buf_size);

    c_clog_info("String: %s", str);
    c_clog_info("String len: %d", c_string_len(str));

    c_string_free(str);
}

#else
#error "This unittests must be called once"
#endif // CLIB_TEST_STRING_H
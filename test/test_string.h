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
    // char* str = "My name is CLib";
    char* str = "ماي نيم ايز خان";
    u32 str_size = strlen(str);
    CString cstr = c_string_new(str, str_size);

    c_clog_info("String size: %d", cstr.size);
    c_clog_info("String len: %d", cstr.len);
}

#else
#error "This unittests must be called once"
#endif // CLIB_TEST_STRING_H
#ifndef CLIB_UNITTEST_H
#define CLIB_UNITTEST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void (*c_test_case_handler)();

typedef struct {
    const char* name;
    c_test_case_handler handler;
} Test_Case;

typedef struct {
    uint32_t max_test_cases_num;
    uint32_t test_cases_num;
    Test_Case* test_cases;
    uint32_t current_test_case_index;
    const char* targeted_test_case_name;
    uint32_t cases_positive;
    uint32_t cases_negative;
    uint32_t checks_positive;
    uint32_t checks_negative;
} CUnit_Test;

typedef enum { TEST_TYPE_CHECK, TEST_TYPE_REQUIRE } CTest_Check_Type;

CUnit_Test
c_test_init(size_t max_test_cases, int argc, const char** argv);

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define c_test_register(unit_test, handler) \
    void handler(CUnit_Test* test); \
    c_test_register_impl(unit_test, handler, #handler)
#else
#define c_test_register(unit_test, handler) \
    void handler(CUnit_Test* test); \
    _Generic((unit_test), \
    CUnit_Test*: _Generic((handler), c_test_case_handler: c_test_register_impl))(unit_test, handler, #handler)
#endif

void
c_test_run(CUnit_Test* self);

#define TEST_CHECK(expr) \
    if(!c_test_check_impl(self, TEST_TYPE_CHECK, expr, #expr, __FILE__, __LINE__)) return;
#define TEST_CHECK_MESSAGE(expr, msg) \
    if(!c_test_check_impl(self, TEST_TYPE_CHECK, expr, msg, __FILE__, __LINE__)) return;
#define TEST_REQUIRE(expr)  \
    if(!c_test_check_impl(self, TEST_TYPE_REQUIRE, expr, #expr, __FILE__, __LINE__)) return;
#define TEST_REQUIRE_MESSAGE(expr, msg) \
    if(!c_test_check_impl(self, TEST_TYPE_REQUIRE, expr, msg, __FILE__, __LINE__)) return;


/// Private functions (DON'T CALL THEM)
void c_test_register_impl(CUnit_Test* self, c_test_case_handler handler, const char* name);
bool c_test_check_impl(CUnit_Test* self, CTest_Check_Type check_type, bool expr, const char* expr_str, const char* file, uint32_t line);

#endif // CLIB_UNITTEST_H
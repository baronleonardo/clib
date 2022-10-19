#include <clib/unittest.h>
#include <clib/config.h>

#include <signal.h>
#include <errno.h>
#include <string.h>

#ifndef o_fprintf
#include <stdio.h>
#define o_fprintf fprintf
#endif // o_fprintf

#ifndef o_printf
#include <stdio.h>
#define o_printf printf
#endif // o_printf

#ifdef _WIN32
#define c_unittest_aligned_alloc(alignment, size) _aligned_malloc(size, alignment)
#define c_unittest_aligned_free _aligned_free
#define c_unittest_aligneof _Alignof
#else
#include <stdalign.h>
#define c_unittest_aligned_alloc(alignment, size) aligned_alloc(alignment, size)
#define c_unittest_aligned_free  free
#define c_unittest_aligneof alignof
#endif

#ifndef C_UNITTEST_NO_COLOR
#define __TEST_COLOR_CHECK_FAILED__     "\x1b[33m"
#define __TEST_COLOR_REQUIRE_FAILED__   "\x1b[31m"
#define __TEST_COLOR_TITLES__           "\x1b[33m"
#define __TEST_COLOR_PASSED__           "\x1b[32m"
#define __TEST_COLOR_FAILED__           "\x1b[31m"
#define __TEST_COLOR_RESET__            "\x1b[0m"
#else
#define __TEST_COLOR_CHECK_FAILED__
#define __TEST_COLOR_REQUIRE_FAILED__
#define __TEST_COLOR_TITLES__
#define __TEST_COLOR_PASSED__
#define __TEST_COLOR_FAILED__
#define __TEST_COLOR_RESET__
#endif // C_UNITTEST_NO_COLOR

// sig_atomic_t __signal_status__;
static CUnit_Test* __unit_test__ = NULL;

static void __c_test_on_fatal_error__(CUnit_Test* self, int signal_number);
static int __c_test_post_run__(CUnit_Test* self);
static void __c_test_on_singal__(int signal_num);

CUnit_Test
c_test_init(size_t max_test_cases, int argc, const char** argv)
{
    CUnit_Test unit_test = {
        .max_test_cases_num = max_test_cases,
        .test_cases = c_unittest_aligned_alloc(c_unittest_aligneof(Test_Case), sizeof(Test_Case) * max_test_cases),
    };

    unit_test.cases_positive = unit_test.cases_negative = unit_test.checks_negative = unit_test.checks_positive = 0;

    if(argc > 1)
    {
        unit_test.targeted_test_case_name = argv[1];
    }

    signal(SIGILL, __c_test_on_singal__);
    signal(SIGABRT, __c_test_on_singal__);
    signal(SIGFPE, __c_test_on_singal__);
    signal(SIGSEGV, __c_test_on_singal__);
    signal(SIGTERM, __c_test_on_singal__);

    return unit_test;
}

/////////////////////////////////////////////////////////////////

void
__c_test_on_singal__(int signal_num)
{
    if(__unit_test__)
    {
        signal(signal_num, SIG_IGN);
        __unit_test__->cases_negative++;
        __c_test_on_fatal_error__(__unit_test__, signal_num);

        // __signal_status__ = signal_num;
        // printf("%d\n", errno);
        // exit(0);
        // puts("opaaaaaaaaaaa");
        // abort();
    }
    else
    {
        signal(signal_num, SIG_DFL);
    }
}

void
__c_test_deinit__(CUnit_Test* self)
{
    c_unittest_aligned_free(self->test_cases);
    self->test_cases = NULL;
}

void
c_test_register_impl(CUnit_Test* self, c_test_case_handler handler, const char* name)
{
    Test_Case test_case = {
        .name = name,
        .handler = handler
    };

    if(self->test_cases_num == self->max_test_cases_num)
    {
        o_fprintf(stderr, "You can not add more test cases, you already have %d\n", self->test_cases_num); 
        return;
    }

    self->test_cases[self->test_cases_num] = test_case;
    self->test_cases_num++;
}

void __c_test_on_fatal_error__(CUnit_Test* self, int sig_number)
{
    const char* sig_names[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        "SIGILL",
        NULL,
        "SIGABRT",
        NULL,
        "SIGFPE",
        NULL,
        NULL,
        "SIGSEGV",
        NULL,
        NULL,
        NULL,
        "SIGTERM"
    };

    o_fprintf(stderr,
        __TEST_COLOR_FAILED__
        "ERROR: signal %s: Test case `%s` failed.\n"
        __TEST_COLOR_RESET__,
        sig_names[sig_number],
        self->test_cases[self->current_test_case_index].name
    );

    int status = __c_test_post_run__(self);
    exit(status);
    // exit(EXIT_FAILURE);
}

int
__c_test_post_run__(CUnit_Test* self)
{
    o_printf("\n"
             "=======================\n"
             __TEST_COLOR_TITLES__
             "Test cases:\n"
             __TEST_COLOR_PASSED__
             "\tpassed: %d\n"
             __TEST_COLOR_FAILED__
             "\tfailed: %d\n"
             __TEST_COLOR_TITLES__
             "Checks:\n"
             __TEST_COLOR_PASSED__
             "\tpassed: %d\n"
             __TEST_COLOR_FAILED__
             "\tfailed: %d\n"
             __TEST_COLOR_RESET__,
             self->cases_positive - self->cases_negative,
             self->cases_negative,
             self->checks_positive,
             self->checks_negative);

    int negatives = self->cases_negative + self->checks_negative;

    __c_test_deinit__(self);

    return negatives;
}

void
c_test_run(CUnit_Test* self)
{
    __unit_test__ = self;

    if(self->targeted_test_case_name)
    {
        for(uint32_t iii = 0; iii < self->test_cases_num; iii++)
        {
            if(strcmp(self->targeted_test_case_name, self->test_cases[iii].name) == 0)
            {
                self->current_test_case_index = iii;
                self->test_cases[iii].handler(self);
                self->cases_positive = 1;
                break;
            }
        }
    }
    else
    {
        self->cases_positive = self->test_cases_num;
        for(uint32_t iii = 0; iii < self->test_cases_num; iii++)
        {
            self->current_test_case_index = iii;
            self->test_cases[iii].handler(self);
        }
    }

    int status = __c_test_post_run__(self);
    exit(status);
}

bool
c_test_check_impl(
    CUnit_Test* self,
    CTest_Check_Type check_type,
    bool expr,
    const char* expr_str,
    const char* file,
    uint32_t line
)
{
    if(check_type == TEST_TYPE_CHECK)
    {
        // self->requires_positive++;

        if(!expr)
        {
            fprintf(stderr,
                __TEST_COLOR_CHECK_FAILED__
                "-- %s:%d\n\t\t%s\n\n"
                __TEST_COLOR_RESET__,
                file, line, expr_str);

            self->checks_negative++;
        }
        else
        {
            self->checks_positive++;
        }
    }
    else if(check_type == TEST_TYPE_REQUIRE)
    {
        if(!expr)
        {
            fprintf(stderr,
                __TEST_COLOR_REQUIRE_FAILED__
                "-- %s:%d\n\t\t%s\n\n"
                __TEST_COLOR_RESET__,
                file, line, expr_str);

            self->checks_negative++;

            o_fprintf(stderr,
                __TEST_COLOR_FAILED__
                "FATAL ERROR: Test case `%s` failed.\n"
                __TEST_COLOR_RESET__,
                self->test_cases[self->current_test_case_index].name
            );

            return false;
        }
        else
        {
            self->checks_positive++;
        }
    }

    return true;
}
#include <clib/unittest.h>

#include "test_string.h"
#include "test_list.h"

int main(int argc, const char* argv[])
{
    CUnit_Test unit_test = c_test_init(100, argc, argv);

    c_test_string(&unit_test);
    c_test_list(&unit_test);

    c_test_run(&unit_test);
}
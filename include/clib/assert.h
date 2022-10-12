#ifndef CLIB_ASSERT_H
#define CLIB_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define c_assert(cmp, msg) if(!(cmp)) { fprintf(stderr, "Assertion Failed: %s:%d -> %s\n%s\n", __FILE__, __LINE__, #cmp, msg); abort(); }

#ifndef NDEBUG
#define c_assert_debug(cmp, msg) c_assert(cmp, msg)
#else
#define c_assert_debug(cmp, msg)
#endif // NDEBUG

#endif // CLIB_ASSERT_H
#ifndef CASSERT_H
#define CASSERT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define cassert(expr) ((void)(expr))
#else
#include <assert.h>
#define cassert(expr) assert((expr))
#endif // NDEBUG

/// @brief these macros will always run in Debug/Release modes
#define __cassert_always_print(expr) (fprintf(stderr, "%s:%d: %s: Assertion `(%s)' failed.\n", __FILE__, __LINE__, __func__, #expr))
#define cassert_always(expr) ((expr) ? 0 : (__cassert_always_print((expr)), abort()))
#define cassert_always_msg(expr, msg) ((expr) ? ((void)0) : (__cassert_always_print(expr), puts((msg)), abort()))
#define cassert_always_perror(expr, msg) ((expr) ? ((void)0) : (__cassert_always_print(expr), perror((msg)), abort()))

#endif // CASSERT_H

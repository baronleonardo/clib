#ifndef CASSERT_H
#define CASSERT_H

#include <assert.h>

#ifdef NDEBUG
#define cassert(expr) ((void)(expr))
#else
#define cassert(expr) assert(expr)
#endif // NDEBUG

#endif // CASSERT_H
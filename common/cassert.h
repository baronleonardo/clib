#ifndef CASSERT_H
#define CASSERT_H

#ifdef NDEBUG
#define cassert(expr) ((void)(expr))
#else
#include <assert.h>
#define cassert(expr) assert(expr)
#endif // NDEBUG

#endif // CASSERT_H
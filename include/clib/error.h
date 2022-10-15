#ifndef CLIB_ERROR_H
#define CLIB_ERROR_H

#include "string.h"

/// @TODO: memory safty (multithreading)

typedef struct CError {
    union {
        bool valid;
        cstr msg;
    };
    i32 code;
} CError;

CError
c_error_get();

void
c_error_set(i32 code, const char* msg);

#endif // CLIB_ERROR_H
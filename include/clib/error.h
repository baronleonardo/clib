#ifndef CLIB_ERROR_H
#define CLIB_ERROR_H

#include "typedef.h"

#define C_NO_ERROR  0

typedef struct CError {
    i32 code;
    const char* msg;
} CError;

void
c_error_set(CError* self, i32 code, const char* msg);

void
c_error_set_no_error(CError* self);

#endif // CLIB_ERROR_H
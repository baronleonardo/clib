#ifndef CLIB_ERROR_H
#define CLIB_ERROR_H

#include "string.h"

/// @TODO: memory safty (multithreading)

typedef struct CError {
    union {
        bool valid;
        CString msg;
    };
    i32 code;
} CError;

CError
c_error_get();

bool
c_error_set(const char* msg, u32 msg_size, i32 code);

#endif // CLIB_ERROR_H
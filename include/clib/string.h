#ifndef CLIB_STRING_H
#define CLIB_STRING_H

#include "typedef.h"
#include "char.h"

/// @brief: A utf-8 string library 

typedef struct CString {
    union{
        uchar* buf;
        bool valid;
    };
    u32 size;   // size in bytes
    u32 len;    // number of characters
} CString;

CString
c_string_new(char* buf, u32 size);

i64
c_string_find(CString* self, CString* token);

i64
c_string_find_char(CString* self, CChar* ch);

// void
// c_string_free(CString* self);

#endif // CLIB_STRING_H
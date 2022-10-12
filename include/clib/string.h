#ifndef CLIB_STRING_H
#define CLIB_STRING_H

#include "typedef.h"
#include "char.h"

/// @brief: A utf-8 null-terminated string library

// typedef struct CString {
//     union{
//         uchar* buf;
//         bool valid;
//     };
//     u32 size;   // size in bytes
//     u32 capacity;
//     bool on_heap;
// } CString;

typedef char* cstr;

cstr
c_string_new(u32 size);

cstr
c_string_new_from_buf(const char* buf, u32 size);

cstr
c_string_copy(cstr self);

u32
c_string_len(cstr self);

/// @brief use this function to update `self` with the new length
///        if you manipulated the string externally (like using `string.h` functions)
void
c_string_update_len(cstr self, u32 new_len);

i64
c_string_find(cstr self, char* token);

i64
c_string_find_char(cstr self, CChar* ch);

cstr
c_string_free(cstr self);

#endif // CLIB_STRING_H
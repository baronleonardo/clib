#ifndef CLIB_STRING_H
#define CLIB_STRING_H

#include "typedef.h"
#include "char.h"

/// @brief: A utf-8 null-terminated string library

typedef struct CString {
    u32 size; // including header and zero terminated
    u32 str_len;
} CString;

typedef char* cstr;

#define cstr_whitespaces " \t\n\v\f\r"

cstr
c_string_new(u32 size);

cstr
c_string_new_from_buf(const char* buf, u32 size);

cstr
c_string_copy(cstr self);

u32
c_string_len(cstr self);

u32
c_string_capacity(cstr self);

/// @brief use this function to update `self` with the new length
///        if you manipulated the string externally (like using `string.h` functions)
void
c_string_update_len(cstr self);

bool
c_string_equal(cstr str1, cstr str2);

void
c_string_trim(cstr self, const char* chars, u32 chars_len);

void
c_string_trim_whitespaces(cstr self);

void
c_string_ltrim(cstr self, const char* chars, u32 chars_len);

void
c_string_ltrim_whitespaces(cstr self);

void
c_string_rtrim(cstr self, const char* chars, u32 chars_len);

void
c_string_rtrim_whitespaces(cstr self);

i64
c_string_find(cstr self, char* token);

i64
c_string_find_char(cstr self, CChar* ch);

cstr
c_string_free(cstr self);

#endif // CLIB_STRING_H
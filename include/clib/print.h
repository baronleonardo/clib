#ifndef CLIB_PRINT_H
#define CLIB_PRINT_H

#include "typedef.h"
#include "config.h"
#include "string.h"

#define MAX_PARAM_NUM 8

typedef struct
{
    size_t params_len;
    cstr params[MAX_PARAM_NUM];      // the max number of parameters
} CPrint_Cache;

CPrint_Cache*
c_string_from_bool(bool val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_cstr(char* val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_i32(i32 val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_i64(i64 val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_double(double val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_long_double(long double val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_char(char val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_u32(u32 val, CPrint_Cache* cache);

CPrint_Cache*
c_string_from_u64(u64 val, CPrint_Cache* cache);

#define to_string(type, buf) _Generic((type),      /* convert types to string */      \
         bool: c_string_from_bool,                  unsigned char: c_string_from_char,        \
        char*: c_string_from_cstr,                    signed char: c_string_from_char,        \
          i16: c_string_from_i32,                             u16: c_string_from_i32,         \
          i32: c_string_from_i32,                             u32: c_string_from_u32,         \
          i64: c_string_from_i64,                             u64: c_string_from_u64,         \
        float: c_string_from_double,                       double: c_string_from_double,         \
  long double: c_string_from_long_double,                    char: c_string_from_char,        \
       uchar*: c_string_from_cstr                                                         \
)(type, buf)

#define printn(format, buf, buf_size, ...) print_(format, buf, buf_size, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define print(format, ...) \
do { \
    char buf_data[PRINT_STR_MAX_LEN]; \
    print_(c_print_format_impl, buf_data, PRINT_STR_MAX_LEN, __VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0); \
} while(0)

#define print_(format_, buf, buf_size, a0, a1, a2, a3, a4, a5, a6, a7, CNT, ...) \
do \
{ \
    CPrint_Cache cache; \
    c_print_prepare_cache_impl(format_, buf, buf_size, &cache); \
    size_t result_size; \
    if(CNT == 1) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a0, &cache)); \
    if(CNT == 2) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a1, to_string(a0, &cache))); \
    if(CNT == 3) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a2, to_string(a1, to_string(a0, &cache)))); \
    if(CNT == 4) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a3, to_string(a2, to_string(a1, to_string(a0, &cache))))); \
    if(CNT == 5) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a4, to_string(a3, to_string(a2, to_string(a1, to_string(a0, &cache)))))); \
    if(CNT == 6) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a5, to_string(a4, to_string(a3, to_string(a2, to_string(a1, to_string(a0, &cache))))))); \
    if(CNT == 7) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a6, to_string(a5, to_string(a4, to_string(a3, to_string(a2, to_string(a1, to_string(a0, &cache)))))))); \
    if(CNT == 8) \
        result_size = c_print_format_impl(format_, buf, buf_size, to_string(a7, to_string(a6, to_string(a5, to_string(a4, to_string(a3, to_string(a2, to_string(a1, to_string(a0, &cache))))))))); \
    \
    if(result_size) puts(buf); \
} \
while(0)


/// Private functions (DON'T CALL THEM)
size_t
c_print_format_impl(uchar* _format, uchar* buf, size_t buf_size, CPrint_Cache* cache);

void
c_print_prepare_cache_impl(uchar* format_, uchar* buf, size_t buf_size, CPrint_Cache* result);

#endif // CLIB_PRINT_H
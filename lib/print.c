// #include <clib/print.h>

// #include <stdlib.h>
// #include <string.h>

// CPrint_Cache*
// c_string_from_bool(bool val, CPrint_Cache* cache)
// {
//     if(!cache)
//     {
//         return NULL;
//     }

//     char* val_str = val ? "true" : "false";
//     size_t val_str_len = strlen(val_str);

//     cache->params[cache->params_len] = str_new_alloc(val_str, cache->allocator);

//     if(cache->params[cache->params_len].valid)
//     {
//         cache->params_len++;
//         return cache;
//     }

//     return NULL;
// }

// CPrint_Cache*
// c_string_from_cstr(char* val, CPrint_Cache* cache)
// {
//     if(!cache || !val)
//     {
//         return NULL;
//     }

//     cache->params[cache->params_len] = str_new_alloc(val, cache->allocator);

//     if(cache->params[cache->params_len].valid)
//     {
//         cache->params_len++;
//         return cache;
//     }

//     return NULL;
// }

// CPrint_Cache*
// c_string_from_i32(i32 val, CPrint_Cache* cache)
// {
//     if(!cache)
//     {
//         return NULL;
//     }

//     cache->params[cache->params_len] = str_from_i32_alloc(val, cache->allocator);

//     if(cache->params[cache->params_len].valid)
//     {
//         cache->params_len++;
//         return cache;
//     }

//     return NULL;

//     return cache;
// }

// CPrint_Cache*
// c_string_from_i64(i64 val, CPrint_Cache* cache)
// {
//     return cache;
// }

// CPrint_Cache*
// c_string_from_double(double val, CPrint_Cache* cache)
// {
//     if(!cache)
//     {
//         return NULL;
//     }

//     const size_t precision = 4;
//     Str str = str_from_double_alloc(val, 4, cache->allocator);
//     cache->params[cache->params_len] = str;

//     if(cache->params[cache->params_len].valid)
//     {
//         cache->params_len++;
//         return cache;
//     }

//     return NULL;

//     return cache;
// }

// CPrint_Cache*
// c_string_from_long double(long double val, CPrint_Cache* cache)
// {
//     return cache;
// }

// CPrint_Cache*
// c_string_from_char(char val, CPrint_Cache* cache)
// {
//     if(!cache)
//     {
//         return NULL;
//     }

//     Str str = str_new_with_len_alloc(&val, 1, cache->allocator);

//     if(str.valid)
//     {
//         return cache;
//     }

//     return NULL;

//     return cache;
// }

// CPrint_Cache*
// c_string_from_u32(u32 val, CPrint_Cache* cache)
// {
//     return cache;
// }

// CPrint_Cache*
// c_string_from_u64(u64 val, CPrint_Cache* cache)
// {
//     return cache;
// }

// void
// c_print_prepare_cache_impl(uchar* format_, uchar* buf, size_t buf_size, CPrint_Cache* result)
// {
//     size_t format_len = strlen(format_);
//     result->arena = arena_create_from(buf + format_len, buf_size - format_len);
//     result->allocator = allocator_arena(&result->arena);
//     result->params_len = 0;
// }

// size_t
// c_print_format_impl(uchar* _format, uchar* buf, size_t buf_size, CPrint_Cache* cache)
// {
//     size_t params_num = 0;
//     uchar* c = _format;
//     uchar* cur_str = c;
//     uchar* param_start = NULL;
//     uchar* _buf = buf;

//     while(*c != '\0')
//     {
//         if(*c == '{')
//         {
//             memcpy(_buf, cur_str, c - cur_str);
//             _buf += c - cur_str;
//             param_start = c;
//         }

//         if(*c == '}')
//         {
//             memcpy(_buf, cache->params[params_num].chunk.mem, cache->params[params_num].chunk.size - 1);
//             _buf += cache->params[params_num].chunk.size - 1;
//             params_num++;
//             cur_str = c + 1;
//         }

//         c++;
//     }

//     if(*cur_str)
//     {
//         strcpy(_buf, cur_str);
//         _buf += strlen(_buf);
//         *_buf = '\0';
//     }

//     // this includes '\0'
//     return _buf - buf;
// }
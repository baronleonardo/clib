#ifndef STR_INTERNAL_H
#define STR_INTERNAL_H

#include <stdint.h>
#include <stddef.h>

#include <str.h>
#include <cassert.h>

/// @brief the design inspired by the wonderful sds
///        https://github.com/antirez/sds
typedef struct {
    size_t capacity;
    size_t len;
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    char data[1];
#else
    char data[];
#endif
} StrMeta;

static inline StrMeta*
str_internal_get_meta(const Str self) {
    cassert(self);
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    static const void* data_offset = &(((StrMeta*)NULL)->data);
    return (StrMeta*)((uint8_t*)self - (uint8_t*)data_offset);
#else
    return (&((StrMeta*)(self))[-1]);
#endif
}

static inline char*
str_internal_search(Str self, const char* cstring, size_t max_len, StrMeta** return_meta, size_t* return_cstring_len) {
    cassert(self);

    if(cstring) {
        StrMeta* meta = str_internal_get_meta(self);
        *return_meta = meta;
        size_t cstring_len = strnlen(cstring, max_len);
        *return_cstring_len = cstring_len;

        for(size_t str_counter = 0; str_counter < meta->len; ++str_counter) {
            if(memcmp(&meta->data[str_counter], cstring, cstring_len) == 0) {
                return &meta->data[str_counter];
            }
        }
    }

    return NULL;
}

#endif // STR_INTERNAL_H

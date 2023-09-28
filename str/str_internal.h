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
} StrMeta;

static inline StrMeta*
str_internal_get_meta(const Str* self) {
    return (&((StrMeta*)(self->data))[-1]);
}

static inline char*
str_internal_get_data(const StrMeta* meta) {
    return (char*)(&(meta[1]));
}

static inline char*
str_internal_search(Str* self, const char* cstring, size_t max_len, StrMeta** return_meta, size_t* return_cstring_len) {
    cassert(self);

    if(cstring) {
        StrMeta* meta = str_internal_get_meta(self);
        *return_meta = meta;
        size_t cstring_len = strnlen(cstring, max_len);
        *return_cstring_len = cstring_len;

        for(size_t str_counter = 0; str_counter < meta->len; ++str_counter) {
            if(memcmp(&(self->data)[str_counter], cstring, cstring_len) == 0) {
                return &(self->data)[str_counter];
            }
        }
    }

    return NULL;
}

#endif // STR_INTERNAL_H

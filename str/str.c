#include <string.h>
#include <stdlib.h>

#include <str.h>
#include "str_internal.h"
#include <cassert.h>

Str*
str_create(const char* cstring, size_t max_len) {
    cassert(max_len > 0);

    size_t cstring_len = strnlen(cstring, max_len);

    StrMeta* meta = (StrMeta*)malloc(sizeof(StrMeta) + cstring_len + 1);
    cassert(meta);

    meta->capacity = cstring_len + 1;
    meta->len = cstring_len;
    cassert(memcpy(str_internal_get_data(meta), cstring, cstring_len));
    str_internal_get_data(meta)[cstring_len] = '\0';

    return str_internal_get_data(meta);
}

void
str_add(Str** self, const char* cstring, size_t max_len) {
    cassert(self && *self);
    cassert(cstring);
    cassert(max_len > 0);

    StrMeta* meta = str_internal_get_meta(*self);
    size_t cstring_len = strnlen(cstring, max_len);

    // resize
    if((meta->len + cstring_len + 1) > meta->capacity) {
        meta = realloc(meta, sizeof(StrMeta) + (meta->capacity + cstring_len));
        cassert(meta);

        meta = str_internal_get_meta(*self);
        meta->capacity += cstring_len;
    }

#if defined(_WIN32)
    cassert(strncpy_s((Str*)str_internal_get_data(meta) + meta->len, meta->capacity - meta->len, cstring, cstring_len) == 0);
#else
    cassert(strncpy(str_internal_get_data(meta) + meta->len, cstring, cstring_len));
#endif
    meta->len += cstring_len;
    str_internal_get_data(meta)[meta->len] = '\0';
}

bool
str_remove(Str* self, const char* cstring, size_t max_len) {
    cassert(self);
    cassert(cstring);
    cassert(max_len > 0);

    StrMeta* meta = NULL;
    size_t cstring_len = 0;
    char* substring_ptr = str_internal_search(self, cstring, max_len, &meta, &cstring_len);

    if(substring_ptr) {
        cassert(memmove(
            substring_ptr,
            substring_ptr + cstring_len,
            (str_internal_get_data(meta) + meta->len) - (substring_ptr + cstring_len))
        );
        *substring_ptr = '\0';
        meta->len -= cstring_len;
        return true;
    } else {
        return false;
    }
}

char*
str_search(Str* self, const char* cstring, size_t max_len) {
    cassert(self);
    cassert(cstring);
    cassert(max_len > 0);

    return str_internal_search(self, cstring, max_len, NULL, NULL);
}

size_t
str_len(const Str* self) {
    cassert(self);

    StrMeta* meta = str_internal_get_meta(self);
    return meta->len;
}

size_t
str_capacity(const Str* self) {
    cassert(self);

    StrMeta* meta = str_internal_get_meta(self);
    return meta->capacity;
}

void
str_destroy(Str** self) {
    cassert(self && *self);

    StrMeta* meta = str_internal_get_meta(*self);
    free(meta);
    *self = NULL;
}

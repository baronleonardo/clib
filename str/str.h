#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef char Str;

Str*
str_create(const char* cstring, size_t max_len);

void
str_add(Str** self, const char* cstring, size_t max_len);

char*
str_search(Str* self, const char* cstring, size_t max_len);

bool
str_remove(Str* self, const char* cstring, size_t max_len);

size_t
str_len(const Str* self);

size_t
str_capacity(const Str* self);

void
str_destroy(Str** self);

#endif // STRING_H

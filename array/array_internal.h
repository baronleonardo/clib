#ifndef LIST_INTERNAL_H
#define LIST_INTERNAL_H

#include "array.h"

#include <string.h>
#include <stdint.h>

/// @brief the design inspired by the wonderful cvector
///        https://github.com/eteran/c-vector
typedef struct {
    size_t capacity;     /// maximum data that can be hold, note: this unit based not bytes based
    size_t len;          /// current length, note: this unit based not bytes based
    size_t element_size; /// size of the unit
} ArrayMeta;

inline static ArrayMeta*
array_internal_get_meta(const Array self) {
    return (&((ArrayMeta*)(self))[-1]);
}

inline static Array*
array_internal_get_data(const ArrayMeta* self) {
    return (Array*)(&self[1]);
}

#endif // LIST_INTERNAL_H

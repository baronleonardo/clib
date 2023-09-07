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
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    uint8_t data[1];      /// the actuall data
#else
    uint8_t data[];      /// the actuall data
#endif
} ArrayMeta;

inline static ArrayMeta*
array_internal_get_meta(const Array self) {
    // static const void* data_offset = &(((ArrayMeta*)NULL)->data);
    // return (ArrayMeta*)(self - data_offset);
    return (&((ArrayMeta*)(self))[-1]);
}

#endif // LIST_INTERNAL_H

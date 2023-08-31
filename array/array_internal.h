#ifndef LIST_INTERNAL_H
#define LIST_INTERNAL_H

#include "array.h"

#include <string.h>
#include <assert.h>

inline static ArrayMeta*
array_internal_get_meta(const Array* self) {
    // static const void* data_offset = &(((ArrayMeta*)NULL)->data);
    // return (ArrayMeta*)(self - data_offset);
    assert(self);
    return (&((ArrayMeta*)(self))[-1]);
}

#endif // LIST_INTERNAL_H

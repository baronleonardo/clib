#include <cassert.h>
#include <stdlib.h>
#include <string.h>

// #include <stdlib.h>
// #include <stdio.h>

#include <array.h>
#include <array_internal.h>

Array*
array_create(size_t element_size) {
    ArrayMeta* meta = calloc(1, sizeof(ArrayMeta) + element_size);
    cassert(meta);

    meta->capacity++;
    meta->element_size = element_size;

    return meta->data;
}

size_t
array_get_len(Array* self) {
    cassert(self);
    return array_internal_get_meta(self)->len;
}

void
array_set_len(Array* self, size_t new_len) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    meta->len = new_len;
}

size_t
array_get_capacity(Array* self) {
    cassert(self);
    return array_internal_get_meta(self)->capacity;
}

Array*
array_set_capacity(Array* self, size_t new_capacity) {
    cassert(self);
    ArrayMeta* meta = array_internal_get_meta(self);

    if(new_capacity == 0) {
        new_capacity++;
    }

    meta = realloc(meta, sizeof(ArrayMeta) + (new_capacity * meta->element_size));
    cassert(meta);

    meta->capacity = new_capacity;

    return meta->data;
}

size_t
array_get_element_size(Array* self) {
    cassert(self);
    return array_internal_get_meta(self)->element_size;
}

Array*
array_push(Array* self, const void* element) {
    cassert(self);
    cassert(element);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len != SIZE_MAX);
  
    if((meta->len + 1) > meta->capacity) {
        meta->capacity *= 2;
        meta = realloc(meta, sizeof(ArrayMeta) + (meta->capacity * meta->element_size));
        cassert(meta);
    }

    cassert(memcpy(meta->data + (meta->len * meta->element_size), element, meta->element_size));

    meta->len++;

    return meta->data;
}

void*
array_pop(Array* self) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len > 0);

    uint8_t* element = meta->data + ((meta->len - 1) * meta->element_size);
    meta->len--;

    return element;
}

void*
array_remove(Array* self, size_t index) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len > 0);

    if(index == (meta->len - 1)) {
        return array_pop(self);
    } else {
        uint8_t* last_element = meta->data + ((meta->len - 1) * meta->element_size);
        uint8_t* element = meta->data + (index * meta->element_size);
        uint8_t* tmp = malloc(meta->element_size);
        cassert(tmp);
        cassert(memcpy(tmp, element, meta->element_size));

        cassert(memmove(
            element,
            element + meta->element_size,
            (meta->len - index) * meta->element_size
        ));

        cassert(memcpy(last_element, tmp, meta->element_size));

        meta->len--;

        free(tmp);
        return last_element;
    }
}

void*
array_remove_range(Array* self, size_t start_index, size_t range_len) {
    cassert(self);
    cassert(range_len);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len > 0);
    cassert(start_index < (meta->len - 1));
    cassert((start_index + range_len) <= meta->len);

    const size_t range_size = range_len * meta->element_size;
    uint8_t* start_ptr = meta->data + (start_index * meta->element_size);

    if((start_index + range_len) == meta->len) {
        meta->len -= range_len;
        return start_ptr;
    } else {
        uint8_t* tmp = malloc(range_size);
        const uint8_t* end_ptr = meta->data + ((start_index + range_len) * meta->element_size);
        size_t right_range_size = (meta->len - (start_index + range_len)) * meta->element_size;

        cassert(memmove(tmp, start_ptr, range_size));
        cassert(memcpy(start_ptr, end_ptr, right_range_size));
        cassert(memcpy(start_ptr + right_range_size, tmp, range_size));

        meta->len -= range_len;

        free(tmp);
        return start_ptr + right_range_size;
    }
}

Array*
array_destroy(Array* self) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    free(meta);

    return NULL;
}

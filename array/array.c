#include <stdlib.h>
#include <string.h>

#include <array.h>
#include <array_internal.h>
#include <cassert.h>

Array
array_create(size_t element_size) {
    return array_create_with_capacity(element_size, 1U);
}

Array
array_create_with_capacity(size_t element_size, size_t capacity) {
    cassert(element_size > 0);
    cassert(capacity > 0);

    ArrayMeta* meta = calloc(1U, sizeof(ArrayMeta) + (capacity * element_size));
    cassert(meta);

    meta->capacity = capacity;
    meta->element_size = element_size;

    return array_internal_get_data(meta);
}

size_t
array_get_len(Array self) {
    cassert(self);

    return array_internal_get_meta(self)->len;
}

void
array_set_len(Array self, size_t new_len) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);

    cassert(new_len < meta->capacity);
    meta->len = new_len;
}

size_t
array_get_capacity(Array self) {
    cassert(self);

    return array_internal_get_meta(self)->capacity;
}

void
array_set_capacity(Array* self, size_t new_capacity) {
    cassert(self && *self);
    cassert(new_capacity > 0);

    ArrayMeta* meta = array_internal_get_meta(*self);

    meta = realloc(meta, sizeof(ArrayMeta) + (new_capacity * meta->element_size));
    cassert(meta);

    meta->capacity = new_capacity;

    *self = array_internal_get_data(meta);
}

size_t
array_get_element_size(Array self) {
    cassert(self);

    return array_internal_get_meta(self)->element_size;
}

void
array_push(Array* self, const void* element) {
    cassert(self && *self);
    cassert(element);

    ArrayMeta* meta = array_internal_get_meta(*self);
    cassert(meta->len < SIZE_MAX);

    if((meta->len + 1U) > meta->capacity) {
        meta->capacity *= 2;
        meta = realloc(meta, sizeof(ArrayMeta) + (meta->capacity * meta->element_size));
        cassert(meta);
    }

    cassert(memcpy(
        (uint8_t*)array_internal_get_data(meta) + (meta->len * meta->element_size),
        element,
        meta->element_size
    ));

    meta->len++;

    *self = array_internal_get_data(meta);
}

void*
array_pop(Array self) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len > 0U);

    uint8_t* element = (uint8_t*)array_internal_get_data(meta) + ((meta->len - 1U) * meta->element_size);
    meta->len--;

    return element;
}

void*
array_remove(Array self, size_t index) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len > 0);

    if(index == (meta->len - 1U)) {
        return array_pop(self);
    } else {
        Array array_data = array_internal_get_data(meta);
        uint8_t* last_element = (uint8_t*)array_data + ((meta->len - 1U) * meta->element_size);
        uint8_t* element = (uint8_t*)array_data + (index * meta->element_size);
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
array_remove_range(Array self, size_t start_index, size_t range_len) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    cassert(meta->len > 0U);
    cassert(start_index < (meta->len - 1U));
    cassert((start_index + range_len) <= meta->len);

    const size_t range_size = range_len * meta->element_size;
    uint8_t* start_ptr = (uint8_t*)array_internal_get_data(meta) + (start_index * meta->element_size);

    if((start_index + range_len) == meta->len) {
        meta->len -= range_len;
        return start_ptr;
    } else {
        uint8_t* tmp = malloc(range_size);
        const uint8_t* end_ptr = (uint8_t*)array_internal_get_data(meta) + ((start_index + range_len) * meta->element_size);
        size_t right_range_size = (meta->len - (start_index + range_len)) * meta->element_size;

        cassert(memmove(tmp, start_ptr, range_size));
        cassert(memcpy(start_ptr, end_ptr, right_range_size));
        cassert(memcpy(start_ptr + right_range_size, tmp, range_size));

        meta->len -= range_len;

        free(tmp);
        return start_ptr + right_range_size;
    }
}

void
array_destroy(Array* self) {
    cassert(self);

    ArrayMeta* meta = array_internal_get_meta(*self);
    free(meta);

    *self = NULL;
}

#include <array.h>
#include <array_internal.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// #include <stdlib.h>
// #include <stdio.h>

Array*
array_create(size_t element_size) {
    ArrayMeta* meta = calloc(1, sizeof(ArrayMeta) + element_size);
    assert(meta);

    meta->capacity++;
    meta->element_size = element_size;

    return meta->data;
}

size_t
array_get_len(Array* self) {
    assert(self);
    return array_internal_get_meta(self)->len;
}

void
array_set_len(Array* self, size_t new_len) {
    assert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    meta->len = new_len;
}

size_t
array_get_capacity(Array* self) {
    assert(self);
    return array_internal_get_meta(self)->capacity;
}

Array*
array_set_capacity(Array* self, size_t new_capacity) {
    assert(self);
    ArrayMeta* meta = array_internal_get_meta(self);

    if(new_capacity == 0) {
        new_capacity++;
    }

    meta = realloc(meta, sizeof(ArrayMeta) + (new_capacity * meta->element_size));
    assert(meta);

    meta->capacity = new_capacity;

    return meta->data;
}

size_t
array_get_element_size(Array* self) {
    assert(self);
    return array_internal_get_meta(self)->element_size;
}

Array*
array_push(Array* self, void* element) {
    assert(self);
    assert(element);

    ArrayMeta* meta = array_internal_get_meta(self);
    assert(meta->len != SIZE_MAX);
  
    if((meta->len + 1) > meta->capacity) {
        meta->capacity *= 2;
        meta = realloc(meta, sizeof(ArrayMeta) + (meta->capacity * meta->element_size));
        assert(meta);
    }

    assert(memcpy(meta->data + (meta->len * meta->element_size), element, meta->element_size));

    meta->len++;

    return meta->data;
}

void*
array_pop(Array* self) {
    assert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    assert(meta->len > 0);

    void* element = meta->data + ((meta->len - 1) * meta->element_size);
    meta->len--;

    return element;
}

void*
array_remove(Array* self, size_t index) {
    assert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    assert(meta->len > 0);

    if(index == (meta->len - 1)) {
        return array_pop(self);
    } else {
        void* last_element = meta->data + ((meta->len - 1) * meta->element_size);
        void* element = meta->data + (index * meta->element_size);

        const size_t element_size = meta->element_size;
        uint8_t tmp[element_size];
        assert(memcpy(tmp, element, meta->element_size));

        assert(memmove(
            element,
            element + meta->element_size,
            (meta->len - index) * meta->element_size
        ));

        assert(memcpy(last_element, tmp, meta->element_size));

        meta->len--;

        return last_element;
    }
}

void*
array_remove_range(Array* self, size_t start_index, size_t range_len) {
    assert(self);
    assert(range_len);

    ArrayMeta* meta = array_internal_get_meta(self);
    assert(meta->len > 0);
    assert(start_index < (meta->len - 1));
    assert((start_index + range_len) <= meta->len);

    const size_t range_size = range_len * meta->element_size;
    void* start_ptr = self + (start_index * meta->element_size);

    if((start_index + range_len) == meta->len) {
        meta->len -= range_len;
        return start_ptr;
    } else {
        uint8_t tmp[range_size];
        void* end_ptr = self + ((start_index + range_len) * meta->element_size);
        size_t right_range_size = (meta->len - (start_index + range_len)) * meta->element_size;

        assert(memmove(tmp, start_ptr, range_size));
        assert(memcpy(start_ptr, end_ptr, right_range_size));
        assert(memcpy(start_ptr + right_range_size, tmp, range_size));

        meta->len -= range_len;

        return start_ptr + right_range_size;
    }
}

Array*
array_destroy(Array* self) {
    assert(self);

    ArrayMeta* meta = array_internal_get_meta(self);
    free(meta);

    return NULL;
}

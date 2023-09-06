#include <stdlib.h>
#include <string.h>

// #include <stdlib.h>
// #include <stdio.h>

#include <array.h>
#include <array_internal.h>
#include <panic.h>

Array
array_create(size_t element_size) {
    return array_create_with_capacity(element_size, 1U, NULL);
}

Array
array_create_with_capacity(size_t element_size, size_t capacity, ArrayError* error) {
    if(capacity == 0U) {
        if(error) *error = ARRAY_ERROR_INVALID_NEW_CAPACITY;
        return NULL;
    }

    ArrayMeta* meta = calloc(1U, sizeof(ArrayMeta) + (capacity * element_size));
    panic_if_memory_failed(meta);

    meta->capacity = capacity;
    meta->element_size = element_size;

    return meta->data;
}

size_t
array_get_len(Array self, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return 0U;
    }

    return array_internal_get_meta(self)->len;
}

void
array_set_len(Array self, size_t new_len, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
    }

    ArrayMeta* meta = array_internal_get_meta(self);
    if(new_len < meta->capacity) {
        meta->len = new_len;
    } else {
        if(error) *error = ARRAY_ERROR_INVALID_NEW_LENGTH;
    }
}

size_t
array_get_capacity(Array self, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return 0U;
    }

    return array_internal_get_meta(self)->capacity;
}

Array
array_set_capacity(Array self, size_t new_capacity, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return NULL;
    }

    ArrayMeta* meta = array_internal_get_meta(self);

    if(new_capacity == 0U) {
        new_capacity++;
    }

    meta = realloc(meta, sizeof(ArrayMeta) + (new_capacity * meta->element_size));
    panic_if_memory_failed(meta);

    meta->capacity = new_capacity;

    return meta->data;
}

size_t
array_get_element_size(Array self, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return 0U;
    }

    return array_internal_get_meta(self)->element_size;
}

Array
array_push(Array self, const void* element, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return NULL;
    }

    if(!element) {
        if(error) *error = ARRAY_ERROR_INVALID_NEW_ELEMENT;
        return NULL;
    }

    ArrayMeta* meta = array_internal_get_meta(self);
    if(meta->len == SIZE_MAX) {
        if(error) *error = ARRAY_ERROR_REACHED_MAX_LENGTH;
        return self;
    }

    if((meta->len + 1U) > meta->capacity) {
        meta->capacity *= 2;
        meta = realloc(meta, sizeof(ArrayMeta) + (meta->capacity * meta->element_size));
        panic_if_memory_failed(meta);
    }

    panic_if_memory_failed(memcpy(meta->data + (meta->len * meta->element_size), element, meta->element_size));

    meta->len++;

    return meta->data;
}

void*
array_pop(Array self, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return NULL;
    }

    ArrayMeta* meta = array_internal_get_meta(self);
    if(meta->len > 0U) {
        uint8_t* element = meta->data + ((meta->len - 1U) * meta->element_size);
        meta->len--;

        return element;
    } else {
        if(error) *error = ARRAY_ERROR_INVALID_LENGTH;
        return NULL;
    }
}

void*
array_remove(Array self, size_t index, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return NULL;
    }

    ArrayMeta* meta = array_internal_get_meta(self);
    if(meta->len > 0U) {
        if(index == (meta->len - 1U)) {
            return array_pop(self, error);
        } else {
            uint8_t* last_element = meta->data + ((meta->len - 1U) * meta->element_size);
            uint8_t* element = meta->data + (index * meta->element_size);
            uint8_t* tmp = malloc(meta->element_size);
            panic_if_memory_failed(tmp);
            panic_if_memory_failed(memcpy(tmp, element, meta->element_size));

            panic_if_memory_failed(memmove(
                element,
                element + meta->element_size,
                (meta->len - index) * meta->element_size
            ));

            panic_if_memory_failed(memcpy(last_element, tmp, meta->element_size));

            meta->len--;

            free(tmp);
            return last_element;
        }
    } else {
        if(error) *error = ARRAY_ERROR_INVALID_LENGTH;
        return NULL;
    }
}

void*
array_remove_range(Array self, size_t start_index, size_t range_len, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return NULL;
    }

    ArrayMeta* meta = array_internal_get_meta(self);
    if(meta->len == 0U) {
        if(error) *error = ARRAY_ERROR_INVALID_LENGTH;
        return 0U;
    }

    if(start_index >= (meta->len - 1U)) {
        if(error) *error = ARRAY_ERROR_INVALID_INDEX;
        return 0U;
    }

    if((start_index + range_len) > meta->len) {
        if(error) *error = ARRAY_ERROR_INVALID_RANGE_LENGTH;
        return 0U;
    }

    const size_t range_size = range_len * meta->element_size;
    uint8_t* start_ptr = meta->data + (start_index * meta->element_size);

    if((start_index + range_len) == meta->len) {
        meta->len -= range_len;
        return start_ptr;
    } else {
        uint8_t* tmp = malloc(range_size);
        const uint8_t* end_ptr = meta->data + ((start_index + range_len) * meta->element_size);
        size_t right_range_size = (meta->len - (start_index + range_len)) * meta->element_size;

        panic_if_memory_failed(memmove(tmp, start_ptr, range_size));
        panic_if_memory_failed(memcpy(start_ptr, end_ptr, right_range_size));
        panic_if_memory_failed(memcpy(start_ptr + right_range_size, tmp, range_size));

        meta->len -= range_len;

        free(tmp);
        return start_ptr + right_range_size;
    }
}

Array
array_destroy(Array self, ArrayError* error) {
    if(!self) {
        if(error) *error = ARRAY_ERROR_WRONG_SELF_OBJECT;
        return NULL;
    }

    ArrayMeta* meta = array_internal_get_meta(self);
    free(meta);

    return NULL;
}

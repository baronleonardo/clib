#include <stdlib.h>
#include <string.h>

#include <map.h>
#include <map_internal.h>
#include <cassert.h>

/// @brief MapElement { bool, key[...], value[...] }
///        MapElement size = sizeof(bool) + sizeof(key) + sizeof(value)

Map
map_create(size_t max_capacity, size_t max_key_size, size_t max_value_size) {
    Map map = {
        .capacity = max_capacity,
        .max_key_size = max_key_size,
        .max_value_size = max_value_size,
        .data = calloc(max_capacity, MAP_ELEMENT_SIZE(&map))
    };
    cassert(map.data);

    return map;
}

bool
map_insert(Map* self, void* key, size_t key_size, void* value, size_t value_size) {
    cassert(self);

    // if the hashmap is full
    if(self->capacity == self->len) {
        return false;
    }

    if(key_size > self->max_key_size) {
        return false;
    }

    if(value_size > self->max_value_size) {
        return false;
    }

    if(key && value) {
        size_t index = map_internal_hasing_algo(key, key_size, self->capacity);
        MapElement* element = map_internal_get_element(self, index);

        // collision
        // find an empty slot using `Quadratic Probing`
        if(element->is_filled && (memcmp(key, element->data, key_size) != 0)) {
            for(size_t iii = 1; element->is_filled; ++iii) {
                index = (index + (iii * iii)) % self->capacity;
                element = map_internal_get_element(self, index);
            }
        }

        void* element_key = element->data;
        void* element_value = (uint8_t*)element_key + self->max_key_size;

        cassert(memcpy(element_key, key, key_size));
        cassert(memcpy(element_value, value, value_size));
        element->is_filled = true;

        self->len++;
        return true;
    }

    return false;
}

void*
map_get(Map* self, void* key, size_t key_size) {
    MapElement* element = map_internal_search_and_get(self, key, key_size);

    if(element) {
        return element->data + self->max_key_size;
    } else {
        return NULL;
    }
}

void*
map_remove(Map* self, void* key, size_t key_size) {
    MapElement* element = map_internal_search_and_get(self, key, key_size);

    if(element) {
        void* element_key = element->data;
        void* element_value = (uint8_t*)element_key + self->max_key_size;

        self->len--;
        element->is_filled = false;
        memset(element_key, 0, self->max_key_size);
        return element_value;
    } else {
        return NULL;
    }
}


size_t
map_len(Map* self) {
    cassert(self);

    return self->len;
}

void
map_destroy(Map* self) {
    cassert(self);

    self->len = 0;
    free(self->data);
}

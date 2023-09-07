#include <stdlib.h>
#include <string.h>

#include <map.h>
#include <map_internal.h>
#include <cassert.h>

Map
map_create(size_t max_capacity, size_t max_key_size, size_t max_value_size) {
    cassert(max_key_size > 0);
    cassert(max_value_size > 0);

    MapMeta* meta = (MapMeta*)calloc(1, sizeof(MapMeta) + (sizeof(Map) + max_key_size + max_value_size));
    cassert(meta);

    meta->capacity = max_capacity;
    meta->max_key_size = max_key_size;
    meta->max_value_size = max_value_size;

    return meta->data;
}

bool
map_insert(Map* self, void* key, size_t key_size, void* value, size_t value_size) {
    cassert(*self);
    cassert(key_size > 0);
    cassert(value);
    cassert(value_size > 0);

    MapMeta* meta = map_internal_get_meta(*self);

    cassert(meta->capacity > meta->len);
    cassert(key_size <= meta->max_key_size);
    cassert(value_size <= meta->max_value_size);

    size_t index = map_internal_hasing_algo(key, key_size, meta->capacity);
    Map element = map_internal_get_element(*self, index);

    // collision
    // find an empty slot using `Quadratic Probing`
    if(element->is_filled && (memcmp(key, element->data, key_size) != 0)) {
        for(size_t iii = 1; element->is_filled; ++iii) {
            index = (index + (iii * iii)) % meta->capacity;
            element = map_internal_get_element(*self, index);
        }
    }

    void* element_key = element->data;
    void* element_value = (uint8_t*)element_key + meta->max_key_size;

    cassert(memcpy(element_key, key, key_size));
    cassert(memcpy(element_value, value, value_size));
    element->is_filled = true;

    meta->len++;
    return true;
}

void*
map_get(const Map self, void* key, size_t key_size) {
    cassert(self);
    cassert(key);
    cassert(key_size > 0);

    Map element = map_internal_search_and_get(self, key, key_size);
    if(element) {
        MapMeta* meta = map_internal_get_meta(self);
        return element->data + meta->max_key_size;
    } else {
        return NULL;
    }
}

void*
map_remove(Map self, void* key, size_t key_size) {
    cassert(self);
    cassert(key);
    cassert(key_size > 0);

    Map element = map_internal_search_and_get(self, key, key_size);

    if(element) {
        MapMeta* meta = map_internal_get_meta(self);
        void* element_key = element->data;
        void* element_value = (uint8_t*)element_key + meta->max_key_size;

        meta->len--;
        element->is_filled = false;
        cassert(memset(element_key, 0, meta->max_key_size));
        return element_value;
    } else {
        return NULL;
    }
}

size_t
map_len(const Map self) {
    cassert(self);

    MapMeta* meta = map_internal_get_meta(self);
    return meta->len;
}

void
map_destroy(Map* self) {
    cassert(*self);

    MapMeta* meta = map_internal_get_meta(*self);
    free(meta);
}

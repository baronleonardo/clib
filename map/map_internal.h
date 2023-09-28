#ifndef MAP_INTERNAL_H
#define MAP_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <map.h>
#include <cassert.h>

#define MAP_ELEMENT_SIZE(map) (sizeof(Map) + sizeof(bool) + (map)->max_key_size + (map)->max_value_size)

typedef struct {
    size_t capacity;
    size_t len;
    size_t max_key_size;
    size_t max_value_size;
} MapMeta;

typedef struct {
    bool* is_filled;
    void* key;
    void* value;
} MapElement;

inline static MapMeta*
map_internal_get_meta(const Map* self) {
    return (&((MapMeta*)(self->data))[-1]);
}

inline static void*
map_internal_get_data(const MapMeta* meta) {
    return (void*)(&meta[1]);
}

static inline size_t
map_internal_hasing_algo(void* key, size_t key_size, size_t capacity) {
    size_t sum = 0;
    for(size_t iii = 0; iii < key_size; ++iii) {
        sum += ((uint8_t*)key)[iii];
    }

    return sum % capacity;
}

static inline MapElement
map_internal_get_element(const Map* self, size_t index) {
    cassert(self);

    MapMeta* meta = map_internal_get_meta(self);
    // Map element = (Map)((uint8_t*)(map_internal_get_data(self)) + (index * MAP_ELEMENT_SIZE(meta)));

    bool* is_filled_addr = (bool*)self->data + 
        (index * (meta->max_key_size + meta->max_value_size + sizeof(bool)));
    uint8_t* key_addr = (uint8_t*)is_filled_addr + sizeof(bool);
    uint8_t* value_addr = key_addr + meta->max_key_size;

    MapElement element = {
        .is_filled = is_filled_addr,
        .key = key_addr,
        .value = value_addr
    };

    return element;
}

static inline MapElement
map_internal_search_and_get(const Map* self, void* key, size_t key_size) {
    cassert(self);

    if(key) {
        MapMeta* meta = map_internal_get_meta(self);
        size_t index = map_internal_hasing_algo(key, key_size, meta->capacity);
        MapElement element = map_internal_get_element(self, index);

        if(*element.is_filled) {
            void* element_key = element.key;

            // check for collision `Quadratic Probing`
            for(size_t iii = 0; memcmp(key, element_key, key_size) != 0; ++iii) {
                if(!*element.is_filled) {
                    return (MapElement){0};
                }

                index = (index + (iii * iii)) % meta->capacity;
                element = map_internal_get_element(self, index);
                element_key = element.key;
            }

            return element;
        }
    }

    return (MapElement){0};
}

#endif // MAP_INTERNAL_H

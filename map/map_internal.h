#ifndef MAP_INTERNAL_H
#define MAP_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <map.h>
#include <cassert.h>

#define MAP_ELEMENT_SIZE(map) (sizeof(MapElement) + (map)->max_key_size + (map)->max_value_size)

typedef struct {
    bool is_filled;
    uint8_t data[]; // key and value
} MapElement;

static inline size_t
map_internal_hasing_algo(void* key, size_t key_size, size_t capacity) {
    size_t sum = 0;
    for(size_t iii = 0; iii < key_size; ++iii) {
        sum += ((uint8_t*)key)[iii];
    }

    return sum % capacity;
}

static inline MapElement*
map_internal_get_element(Map* self, size_t index) {
    cassert(self);

    MapElement* element = (MapElement*)((uint8_t*)self->data + (index * MAP_ELEMENT_SIZE(self)));

//     bool* is_filled_addr = (bool*)self->data + 
//         (index * (self->max_key_size + self->max_value_size + sizeof(bool)));
//     uint8_t* key_addr = (uint8_t*)is_filled_addr + sizeof(bool);
//     uint8_t* value_addr = key_addr + self->max_key_size;

//     MapElement map_element = {
//         .is_filled = is_filled_addr,
//         .key = key_addr,
//         .value = value_addr
//     };

    return element;
}

static inline MapElement*
map_internal_search_and_get(Map* self, void* key, size_t key_size) {
    cassert(self);

    if(key) {
        size_t index = map_internal_hasing_algo(key, key_size, self->capacity);
        MapElement* element = map_internal_get_element(self, index);

        if(element->is_filled) {
            void* element_key = element->data;

            // check for collision `Quadratic Probing`
            for(size_t iii = 0; memcmp(key, element_key, key_size) != 0; ++iii) {
                if(!element->is_filled) {
                    return NULL;
                }

                index = (index + (iii * iii)) % self->capacity;
                element = map_internal_get_element(self, index);
                element_key = element->data;
            }

            return element;
        }
    }

    return NULL;
}

#endif // MAP_INTERNAL_H

#ifndef MAP_INTERNAL_H
#define MAP_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <map.h>
#include <cassert.h>

#define MAP_ELEMENT_SIZE(map) (sizeof(Map) + (map)->max_key_size + (map)->max_value_size)

typedef struct {
    size_t capacity;
    size_t len;
    size_t max_key_size;
    size_t max_value_size;
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    struct Map data[1];
#else
    struct Map data[];
#endif
} MapMeta;

inline static MapMeta*
map_internal_get_meta(const Map self) {
    cassert(self);
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    static const void* data_offset = &(((MapMeta*)NULL)->data);
    return (MapMeta*)((uint8_t*)self - (uint8_t*)data_offset);
#else
    return (&((MapMeta*)(self))[-1]);
#endif
}

static inline size_t
map_internal_hasing_algo(void* key, size_t key_size, size_t capacity) {
    size_t sum = 0;
    for(size_t iii = 0; iii < key_size; ++iii) {
        sum += ((uint8_t*)key)[iii];
    }

    return sum % capacity;
}

static inline Map
map_internal_get_element(Map self, size_t index) {
    cassert(self);

    MapMeta* meta = map_internal_get_meta(self);
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)   // MSVC on windows
    Map element = (Map)((uint8_t*)(meta->data) - sizeof(uint8_t) + (index * MAP_ELEMENT_SIZE(meta)));
#else
    Map element = (Map)((uint8_t*)(meta->data) + (index * MAP_ELEMENT_SIZE(meta)));
#endif

//     bool* is_filled_addr = (bool*)self->data + 
//         (index * (self->max_key_size + self->max_value_size + sizeof(bool)));
//     uint8_t* key_addr = (uint8_t*)is_filled_addr + sizeof(bool);
//     uint8_t* value_addr = key_addr + self->max_key_size;

//     Map map_element = {
//         .is_filled = is_filled_addr,
//         .key = key_addr,
//         .value = value_addr
//     };

    return element;
}

static inline Map
map_internal_search_and_get(Map self, void* key, size_t key_size) {
    cassert(self);

    if(key) {
        MapMeta* meta = map_internal_get_meta(self);
        size_t index = map_internal_hasing_algo(key, key_size, meta->capacity);
        Map element = map_internal_get_element(self, index);

        if(element->is_filled) {
            void* element_key = element->data;

            // check for collision `Quadratic Probing`
            for(size_t iii = 0; memcmp(key, element_key, key_size) != 0; ++iii) {
                if(!element->is_filled) {
                    return NULL;
                }

                index = (index + (iii * iii)) % meta->capacity;
                element = map_internal_get_element(self, index);
                element_key = element->data;
            }

            return element;
        }
    }

    return NULL;
}

#endif // MAP_INTERNAL_H

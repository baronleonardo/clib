#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct Map {
    bool is_filled;
    uint8_t data[1]; // key and value, the actual size is `max_key_size + max_value_size`
};
typedef struct Map* Map;

Map
map_create(size_t max_capacity, size_t max_key_size, size_t max_value_size);

bool
map_insert(Map* self, void* key, size_t key_size, void* value, size_t value_size);

void*
map_get(const Map self, void* key, size_t key_size);

void*
map_remove(Map self, void* key, size_t key_size);

size_t
map_len(const Map self);

void
map_destroy(Map* self);

#endif // MAP_H

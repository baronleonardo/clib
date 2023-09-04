#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    size_t capacity;
    size_t len;
    size_t max_key_size;
    size_t max_value_size;
    void* data;
} Map;

enum MapError {
    MapError_none,
    MapError_insert_large_key_size,
    MapError_insert_large_value_size,
    MapError_insert_reached_max_capacity,
};

Map
map_create(size_t max_capacity, size_t max_key_size, size_t max_value_size);

bool
map_insert(Map* self, void* key, size_t key_size, void* value, size_t value_size);

void*
map_get(Map* self, void* key, size_t key_size);

void*
map_remove(Map* self, void* key, size_t key_size);

size_t
map_len(Map* self);

void
map_destroy(Map* self);

#endif // MAP_H

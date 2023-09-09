#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef void* Map;

/// @brief Every element of Map has shape of
///        Element {
///            bool is_filled,
///            uint8_t key[max_key_size],
///            uint8_t value[max_value_size],
///        }

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

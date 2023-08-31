#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include <stddef.h>

// #include <stdlib.h>
// #include <stdio.h>

// [[maybe_unused]] static size_t malloc_index = 0;
// #define ccalloc(i, s) (calloc(i, s))
// #undef calloc
// #define calloc(i, s) (printf("allocation #%ld\n", malloc_index++), ccalloc(i, s))

// #define mmalloc(i, s) (malloc(s))
// #undef malloc
// #define malloc(i, s) (printf("allocation #%ld\n", malloc_index++), mmalloc(s))

// #define ffree(s) (free(s))
// #undef free
// #define free(s) (printf("free - remmaining: %ld\n", --malloc_index), free(s))

/// @brief the design inspired by the wonderful cvector
///        https://github.com/eteran/c-vector
typedef struct {
    size_t capacity;     /// maximum data that can be hold, note: this unit based not bytes based
    size_t len;          /// current length, note: this unit based not bytes based
    size_t element_size; /// size of the unit
    uint8_t data[];      /// the actuall data
} ArrayMeta;

/// @brief you can always cast 'Array*' to anytype you like
///        and use it as oridinary heap allocated array
typedef void Array;

/// @brief create an array with capacity of 1, and element_size
///        the meta data will be create followed by the data itself
///        <pre>
///        +----------+-----+--------------+---------+
///        | capacity | len | element_size | data... |
///        +----------+-----+--------------+---------+
///                                        ^
///                                        | user's pointer
///        </pre>
/// @param element_size 
/// @return 
Array*
array_create(size_t element_size);


/// @brief get array length
/// @param self 
/// @return array length
size_t
array_get_len(Array* self);


/// @brief set array length
///        [beware this is DANGEROUS]
///        this is useful if you want
///        to manipulate the data by yourself
/// @param self 
/// @param new_len 
void
array_set_len(Array* self, size_t new_len);


/// @brief get array capacity
///        this will return the capacity in 'element_size' wise
///        example: 'capacity = 10' means
///                 we can have up to '10 * element_size' bytes
/// @param self 
/// @return the capacity of the array
size_t
array_get_capacity(Array* self);


/// @brief set capacity
///        [beware this is DANGEROUS]
///        this is useful if you want
///        to manipulate the data by yourself
/// @param self 
/// @param new_capacity 
/// @return same or new Array due to resizing
///         [you MUST replace the new Array with this]
Array*
array_set_capacity(Array* self, size_t new_capacity);


/// @brief get elemet_size in bytes
/// @param self 
/// @return elemet_size in bytes
size_t
array_get_element_size(Array* self);


/// @brief push one element at the end
/// @param self 
/// @param element if you want to push literals (example: 3, 5 or 10 ...)
///                array_push(array, &(int){3});
/// @return same or new Array due to resizing
///         [you MUST replace the new Array with this]
Array*
array_push(Array* self, void* element);


/// @brief pop one element from the end
///        [this will NOT resize the array]
/// @param self 
/// @return pointer of the popped element
void*
array_pop(Array* self);


/// @brief remove element from Array
///        [beware this function is costy]
/// @param self 
/// @param index index to be removed
/// @return the removed element
void*
array_remove(Array* self, size_t index);


/// @brief remove a range of elements from Array
/// @param self 
/// @param start_index 
/// @param range_len range length
/// @return the start of the removed range
void*
array_remove_range(Array* self, size_t start_index, size_t range_len);

/// @brief destroy the array from the memory
/// @param self 
/// @return this will always return NULL
///         best practice: array = array_destroy(array);
Array*
array_destroy(Array* self);

#endif // ARRAY_H

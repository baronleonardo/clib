#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef enum {
    ARRAY_ERROR_NONE,
    ARRAY_ERROR_INVALID_NEW_CAPACITY,
    ARRAY_ERROR_INVALID_NEW_LENGTH,
    ARRAY_ERROR_INVALID_LENGTH,
    ARRAY_ERROR_INVALID_RANGE_LENGTH,
    ARRAY_ERROR_INVALID_NEW_ELEMENT,
    ARRAY_ERROR_INVALID_INDEX,
    ARRAY_ERROR_REACHED_MAX_LENGTH,
    ARRAY_ERROR_WRONG_SELF_OBJECT,
} ArrayError;

/// @brief you can always cast 'Array' to anytype you like
///        and use it as oridinary heap allocated array
typedef void* Array;

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
Array
array_create(size_t element_size);


/// @brief same as `array_create` but with allocating capacity
/// @param element_size 
/// @param capacity maximum number of elements to be allocated, minimum capacity is 1
/// @param error [This could be NULL] return error if exists
/// @return 
Array
array_create_with_capacity(size_t element_size, size_t capacity, ArrayError* error);


/// @brief get array length
/// @param self 
/// @param error [This could be NULL] return error if exists
/// @return array length
size_t
array_get_len(Array self, ArrayError* error);


/// @brief set array length
///        [beware this is DANGEROUS]
///        this is useful if you want
///        to manipulate the data by yourself
/// @param self 
/// @param new_len 
/// @param error [This could be NULL] return error if exists
void
array_set_len(Array self, size_t new_len, ArrayError* error);


/// @brief get array capacity
///        this will return the capacity in 'element_size' wise
///        example: 'capacity = 10' means
///                 we can have up to '10 * element_size' bytes
/// @param self 
/// @param error [This could be NULL] return error if exists
/// @return the capacity of the array
size_t
array_get_capacity(Array self, ArrayError* error);


/// @brief set capacity
///        [beware this is DANGEROUS]
///        this is useful if you want
///        to manipulate the data by yourself
/// @param self 
/// @param new_capacity 
/// @param error [This could be NULL] return error if exists
/// @return same or new Array due to resizing
///         [you MUST replace the new Array with this]
Array
array_set_capacity(Array self, size_t new_capacity, ArrayError* error);


/// @brief get elemet_size in bytes
/// @param self 
/// @param error [This could be NULL] return error if exists
/// @return elemet_size in bytes
size_t
array_get_element_size(Array self, ArrayError* error);


/// @brief push one element at the end
/// @param self 
/// @param element if you want to push literals (example: 3, 5 or 10 ...)
///                array_push(array, &(int){3});
/// @param error [This could be NULL] return error if exists
/// @return same or new Array due to resizing
///         [you MUST replace the new Array with this]
Array
array_push(Array self, const void* element, ArrayError* error);


/// @brief pop one element from the end
///        [this will NOT resize the array]
/// @param self 
/// @param error [This could be NULL] return error if exists
/// @return pointer of the popped element
void*
array_pop(Array self, ArrayError* error);


/// @brief remove element from Array
///        [beware this function is costy]
/// @param self 
/// @param index index to be removed
/// @param error [This could be NULL] return error if exists
/// @return the removed element
void*
array_remove(Array self, size_t index, ArrayError* error);


/// @brief remove a range of elements from Array
/// @param self 
/// @param start_index 
/// @param range_len range length
/// @param error [This could be NULL] return error if exists
/// @return the start of the removed range
void*
array_remove_range(Array self, size_t start_index, size_t range_len, ArrayError* error);

/// @brief destroy the array from the memory
/// @param self 
/// @param error [This could be NULL] return error if exists
/// @return this will always return NULL
///         best practice: array = array_destroy(array);
Array
array_destroy(Array self, ArrayError* error);

#endif // ARRAY_H

#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

/// @brief you can always cast 'Array' to anytype you like
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


/// @brief same as `array_create` but with allocating capacity
/// @param element_size 
/// @param capacity maximum number of elements to be allocated, minimum capacity is 1
/// @return 
Array*
array_create_with_capacity(size_t element_size, size_t capacity);


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
/// @param self address of self
/// @param new_capacity 
void
array_set_capacity(Array** self, size_t new_capacity);


/// @brief get elemet_size in bytes
/// @param self 
/// @return elemet_size in bytes
size_t
array_get_element_size(Array* self);


/// @brief push one element at the end
/// @param self pointer to self
/// @param element if you want to push literals (example: 3, 5 or 10 ...)
///                array_push(array, &(int){3});
void
array_push(Array** self, const void* element);


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
void
array_destroy(Array** self);

#endif // ARRAY_H

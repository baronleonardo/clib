#ifndef CLIB_LIST_H
#define CLIB_LIST_H

#include "typedef.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct CList {
    u32 capacity;
    u32 len;
    u32 element_size;
} CList;

#define c_list_new(T) c_list_new_impl(sizeof(T), 0)

#define c_list_new_with_capacity(T, capacity) c_list_new_impl(sizeof(T), capacity)

#define c_list_new_from_array(T, arr, arr_len) c_list_new_from_array_impl(sizeof(T), arr, arr_len)

#define c_list_push(self, elem) (c_list_make_space_impl((void**)&self), self[(((CList *)self) - 1)->len++] = elem)

#define c_list_pop(self) (c_list_pop_validation_impl(self), self[(((CList *)self) - 1)->len-- - 1])

#define c_list_capacity(self) (((CList *)self) - 1)->capacitiy

#define c_list_set_capacity(self, new_capacity) c_list_set_capacity_impl((void**)&self, new_capacity)

#define c_list_get(self, index) (, self[index])

#define c_list_len(self) (((CList *)self) - 1)->len

#define c_list_free(self) c_list_free_impl((void**)&self)

/// Private functions (DON'T CALL THEM)
void* c_list_new_impl(u32 element_size, u32 capacity);
void* c_list_new_from_array_impl(u32 element_size, void* arr, u32 arr_len);
void c_list_make_space_impl(void** self);
void c_list_set_capacity_impl(void** self, u32 new_capacity);
void c_list_free_impl(void** self);
void c_list_pop_validation_impl(void* self);
void c_list_get_validation_impl(void* self);

#endif // CLIB_LIST_H
#include <clib/list.h>
#include <clib/config.h>
#include <clib/assert.h>

#include <stdlib.h>

void*
c_list_new_impl(u32 element_size, u32 capacity)
{
    CList* list_header = malloc(sizeof(CList) + capacity * element_size);
    if(list_header)
    {
        list_header->capacity = capacity;
        list_header->len = 0;
        list_header->element_size = element_size;
        return list_header + 1;
    }
    c_assert(list_header, ERROR_MEM_ALLOC);
    
    return null;
}

void*
c_list_new_from_array_impl(u32 element_size, void* arr, u32 arr_len)
{
    void* list = c_list_new_impl(element_size, arr_len);
    c_assert_debug(list, "");

    CList* list_header = (((CList*)(list)) - 1);
    list_header->len = list_header->capacity;

    void* mem_copy_status = memcpy(list, arr, arr_len * element_size);
    c_assert(mem_copy_status, ERROR_MEM_ALLOC);

    return list;
}

void
c_list_make_space_impl(void** self)
{
    CList* list_header = (((CList*)(*self)) - 1);
    if (list_header->capacity <= list_header->len)
    {
        list_header->capacity = list_header->capacity == 0 ? 1 : list_header->capacity * 2;
        c_list_set_capacity_impl(self, list_header->capacity);
    }
}

void c_list_set_capacity_impl(void** self, u32 new_capacity)
{
    CList* list_header = (((CList*)(*self)) - 1);
    list_header = realloc(list_header, sizeof(CList) + list_header->capacity * list_header->element_size);
    c_assert(list_header, ERROR_MEM_ALLOC);
    *self  = (void*)(list_header + 1);
}

void
c_list_pop_validation_impl(void* self)
{
    c_assert_debug(self, "");

    CList* list_header = (((CList *)self) - 1);
    c_assert_debug(list_header->len > 0, "");
}

void c_list_get_validation_impl(void* self, u32 index)
{
    c_assert_debug(self, "");

    CList* list_header = (((CList *)self) - 1);
    c_assert_debug(index < list_header->len, "");
}

void
c_list_free_impl(void** self)
{
    c_assert_debug(self, "");
    CList* list_header = (((CList *)*self) - 1);
    free(list_header);
    *self = null;
}
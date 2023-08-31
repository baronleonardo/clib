#ifndef CLIB_ALLOCATOR_H
#define CLIB_ALLOCATOR_H

#include "typedef.h"

typedef struct CAllocator_Impl CAllocator_Impl;

typedef struct CAllocator
{
    void* mem;
    CAllocator_Impl* impl;
} CAllocator;

typedef struct CMemChunk {
    union {
        void* mem;
        bool valid;
    };
    usize size;
} CMemChunk;

// default allocator
CAllocator
c_allocator_get_default_allocator();

void
c_allocator_set_default_allocator(CAllocator allocator);

CAllocator
c_allocator_restore_default_allocator();

CMemChunk
c_allocator_default_alloc(CAllocator* self, usize size);

CMemChunk
c_allocator_default_realloc(CAllocator* self, CMemChunk old_mem, usize new_size);

void
c_allocator_default_free(CAllocator* self, CMemChunk* mem);

CMemChunk
c_allocator_default_alloc_aligned(CAllocator* self, usize alignment, usize size);

CMemChunk
c_allocator_default_realloc_aligned(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size);

void
c_allocator_default_free_aligned(CAllocator* self, CMemChunk* mem);


// Arena allocator
/// @note: make sure that that the size of data you want to push to `buf` is equal `size` + sizeof(usize)
typedef struct CAllocator_Arena {
    usize size;
    void* cur_pos;
} CAllocator_Arena;

CAllocator
c_allocator_arena_init(void* buf, usize size);

CMemChunk
c_allocator_arena_alloc(CAllocator* self, usize size);

CMemChunk
c_allocator_arena_alloc_aligned(CAllocator* self, usize alignment, usize size);

CMemChunk
c_allocator_arena_realloc(CAllocator* self, CMemChunk old_mem,  usize new_size);

CMemChunk
c_allocator_arena_realloc_aligned(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size);

usize
c_allocator_arena_current_allocated_size(CAllocator* self);

usize
c_allocator_arena_current_remaining_size(CAllocator* self);

#endif // CLIB_ALLOCATOR_H
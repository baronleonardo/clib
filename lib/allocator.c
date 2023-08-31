#include <clib/allocator.h>
#include <clib/typedef.h>
#include <clib/assert.h>
#include <clib/config.h>

#include <stdlib.h>
#include <string.h>

typedef CMemChunk(*c_alloctor_alloc)(CAllocator* self, usize size);
typedef CMemChunk(*c_alloctor_realloc)(CAllocator* self, CMemChunk old_mem, usize new_size);
typedef void(*c_alloctor_free)(CAllocator* self, CMemChunk* mem);
typedef CMemChunk(*c_alloctor_alloc_aligned)(CAllocator* self, usize alignment, usize size);
typedef CMemChunk(*c_alloctor_realloc_aligned)(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size);
typedef void(*c_alloctor_free_aligned)(CAllocator* self, CMemChunk* mem);

typedef struct CAllocator_Impl
{
    c_alloctor_alloc alloc;
    c_alloctor_realloc realloc;
    c_alloctor_free free;
    c_alloctor_alloc_aligned alloc_aligned;
    c_alloctor_realloc_aligned realloc_aligned;
    c_alloctor_free_aligned free_aligned;
} CAllocator_Impl;

/****************************** Default allocator ******************************/
#ifdef _WIN32
#define c_allocator_default_aligned_alloc(alignment, size)              _aligned_malloc(size, alignment)
#define c_allocator_default_aligned_realloc(mem, alignment, size)       _aligned_realloc(mem, size, alignment)
#define c_allocator_default_aligned_free                                _aligned_free
#define c_allocator_default_aligneof                                    _Alignof
#else
#include <stdalign.h>
#define c_allocator_default_aligned_alloc(alignment, size)              aligned_alloc(alignment, size)
#define c_allocator_default_aligned_realloc(mem, alignment, new_size)   realloc(mem, new_size)
#define c_allocator_default_aligned_free                                free
#define c_allocator_default_aligneof                                    alignof
#endif

static CMemChunk c_allocator_default_alloc_impl(CAllocator* self, usize size);
static CMemChunk c_allocator_default_realloc_impl(CAllocator* self, CMemChunk old_mem, usize new_size);
static void c_allocator_default_free_impl(CAllocator* self, CMemChunk* mem);
static CMemChunk c_allocator_default_alloc_aligned_impl(CAllocator* self, usize alignment, usize size);
static CMemChunk c_allocator_default_realloc_aligned_impl(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size);
static void c_allocator_default_free_aligned_impl(CAllocator* self, CMemChunk* mem);

static CAllocator_Impl c_default_allocator_impl = {
    .alloc = c_allocator_default_alloc_impl,
    .realloc = c_allocator_default_realloc_impl,
    .free = c_allocator_default_free_impl,
    .alloc_aligned = c_allocator_default_alloc_aligned_impl,
    .realloc_aligned = c_allocator_default_realloc_aligned_impl,
    .free_aligned = c_allocator_default_free_aligned_impl,
};
static CAllocator c_default_allocator = { .mem = null, .impl = &c_default_allocator_impl };

CAllocator c_allocator_get_default_allocator()
{
    return c_default_allocator;
}

void
c_allocator_set_default_allocator(CAllocator allocator)
{
    c_default_allocator = allocator;
}

CAllocator
c_allocator_restore_default_allocator()
{
    c_default_allocator = (CAllocator){ .mem = null, .impl = &c_default_allocator_impl };
    return c_default_allocator;
}

CMemChunk
c_allocator_default_alloc(CAllocator* self, usize size)
{
    c_assert_debug(self, "");
    c_assert_debug(self->impl, "");
    if(self->impl->alloc)
    {
        return self->impl->alloc(self, size);
    }

    return (CMemChunk) {};
}

CMemChunk
c_allocator_default_realloc(CAllocator* self, CMemChunk old_mem, usize new_size)
{
    c_assert_debug(self, "");
    c_assert_debug(self->impl, "");
    if(self->impl->realloc)
    {
        return self->impl->realloc(self, old_mem, new_size);
    }

    return (CMemChunk) {};
}

void
c_allocator_default_free(CAllocator* self, CMemChunk* mem)
{
    c_assert_debug(self, "");
    c_assert_debug(self->impl, "");
    c_assert_debug(self->mem, "");
    if(self->impl->free)
    {
        self->impl->free(self, mem);
    }
}

CMemChunk
c_allocator_default_alloc_aligned(CAllocator* self, usize alignment, usize size)
{
    c_assert_debug(self, "");
    c_assert_debug(self->impl, "");
    if(self->impl->alloc_aligned)
    {
        return self->impl->alloc_aligned(self, alignment, size);
    }

    return (CMemChunk) {};
}

CMemChunk
c_allocator_default_realloc_aligned(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size)
{
    c_assert_debug(self, "");
    c_assert_debug(self->impl, "");
    if(self->impl->realloc_aligned)
    {
        return self->impl->realloc_aligned(self, old_mem, alignment, new_size);
    }

    return (CMemChunk) {};
}

void
c_allocator_default_free_aligned(CAllocator* self, CMemChunk* mem)
{
    c_assert_debug(self, "");
    c_assert_debug(self->impl, "");
    c_assert_debug(self->mem, "");
    if(self->impl->free_aligned)
    {
        self->impl->free_aligned(self, mem);
    }
}

/******************************* Arena allocator *******************************/
static CAllocator_Impl c_allocator_arena_impl = {
    .alloc = c_allocator_arena_alloc,
    .realloc = c_allocator_arena_realloc,
    .free = null,
    .alloc_aligned = c_allocator_arena_alloc_aligned,
    .realloc_aligned = null,
    .free_aligned = null,
};

CAllocator
c_allocator_arena_init(void* buf, usize size)
{
    c_assert(size > sizeof(CAllocator_Arena), "size should be greater than the need size + sizeof(CAllocator_Arena)");
    if(buf && size > 0)
    {
        CAllocator allocator = { .impl = &c_allocator_arena_impl, .mem = buf };
        *(CAllocator_Arena*)allocator.mem = (CAllocator_Arena) { .size = size, .cur_pos = (CAllocator_Arena*)allocator.mem + 1 };

        return allocator;
    }
    else
    {
        return (CAllocator){};
    }
}

CMemChunk
c_allocator_arena_alloc(CAllocator* self, usize size)
{
    c_assert_debug(self, "");
    c_assert(self->mem, "Arena Allocator: Can not handle empty memory");

    CAllocator_Arena* arena_manager = (CAllocator_Arena*)self->mem;
    usize remaining_size = arena_manager->size - (arena_manager->cur_pos - self->mem);
    if(arena_manager->size > remaining_size)
    {
        void* new_chunk = arena_manager->cur_pos = (byte*)arena_manager->cur_pos + size;
        return (CMemChunk) { .mem = new_chunk, .size = size };
    }
    else
    {
        return (CMemChunk) {};
    }
}

CMemChunk
c_allocator_arena_alloc_aligned(CAllocator* self, usize alignment, usize size)
{
    c_assert_debug(self, "");
    c_assert(self->mem, "Arena Allocator: Can not handle empty memory");

    CAllocator_Arena* arena_manager = (CAllocator_Arena*)self->mem;
    usize remaining_size = 0;

    if((usize)arena_manager->cur_pos % alignment != 0)
    {
        usize diff = (usize)arena_manager->cur_pos / alignment;
        void* new_loc = (void*)(alignment * diff);
        remaining_size = size - (new_loc - self->mem);
    }
    else
    {
        remaining_size = size - (arena_manager->cur_pos - self->mem);
    }

    if(size > remaining_size)
    {
        void* new_chunk = arena_manager->cur_pos = (byte*)arena_manager->cur_pos + size;
        return (CMemChunk) { .mem = new_chunk, .size = size };
    }
    else
    {
        return (CMemChunk) {};
    }
}

CMemChunk
c_allocator_arena_realloc(CAllocator* self, CMemChunk old_mem, usize new_size)
{
    c_assert_debug(self, "");
    c_assert(self->mem, "Arena Allocator: Can not handle empty memory");

    CAllocator_Arena* arena_manager = (CAllocator_Arena*)self->mem;

    if(old_mem.mem + old_mem.size == arena_manager->cur_pos)
    {
        usize remaining_size = new_size - (old_mem.mem - self->mem);
        if(new_size <= remaining_size)
        {
            void* new_chunk = arena_manager->cur_pos = (byte*)arena_manager->cur_pos + new_size;
            return (CMemChunk){ new_chunk, new_size };
        }
        else
        {
            return (CMemChunk) {};
        }
    }
    else
    {
        CMemChunk new_mem_chunck = c_allocator_arena_alloc(self, new_size);
        void* copy_status = memcpy(new_mem_chunck.mem, old_mem.mem, old_mem.size);
        c_assert(copy_status, ERROR_MEM_ALLOC);

        return new_mem_chunck;
    }
}

CMemChunk
c_allocator_arena_realloc_aligned(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size)
{
    c_assert_debug(self, "");
    c_assert(self->mem, "Arena Allocator: Can not handle empty memory");

    CAllocator_Arena* arena_manager = (CAllocator_Arena*)self->mem;

    if(old_mem.mem + old_mem.size == arena_manager->cur_pos)
    {
        usize remaining_size = new_size - (old_mem.mem - self->mem);
        if(new_size <= remaining_size)
        {
            void* new_chunk = arena_manager->cur_pos = (byte*)arena_manager->cur_pos + new_size;
            return (CMemChunk){ new_chunk, new_size };
        }
        else
        {
            return (CMemChunk) {};
        }
    }
    else
    {
        CMemChunk new_mem_chunck = c_allocator_arena_alloc_aligned(self, alignment, new_size);
        void* copy_status = memcpy(new_mem_chunck.mem, old_mem.mem, old_mem.size);
        c_assert(copy_status, ERROR_MEM_ALLOC);

        return new_mem_chunck;
    }
}

usize
c_allocator_arena_current_allocated_size(CAllocator* self)
{
    c_assert_debug(self, "");
    c_assert_debug(self->mem, "");

    CAllocator_Arena* arena_manager = (CAllocator_Arena*)self->mem;
    usize current_size = (arena_manager->cur_pos - self->mem);

    return current_size;
}

usize
c_allocator_arena_current_remaining_size(CAllocator* self)
{
    c_assert_debug(self, "");
    c_assert_debug(self->mem, "");

    CAllocator_Arena* arena_manager = (CAllocator_Arena*)self->mem;
    usize remaining_size = arena_manager->size - (arena_manager->cur_pos - self->mem);

    return remaining_size;
}

/******************************** Private functions ********************************/

CMemChunk
c_allocator_default_alloc_impl(CAllocator* self, usize size)
{
    c_assert_debug(self, "");
    c_assert(!self->mem, "Can not allocate on an allocated memory");

    self->mem = malloc(size);
    c_assert(self->mem, ERROR_MEM_ALLOC);

    return (CMemChunk) { .mem = self->mem, .size = size };
}

CMemChunk
c_allocator_default_realloc_impl(CAllocator* self, CMemChunk old_mem, usize new_size)
{
    c_assert_debug(self, "");
    c_assert_debug(self->mem, "");

    void* new_mem = realloc(old_mem.mem, new_size);
    c_assert(new_mem, ERROR_MEM_ALLOC);
    self->mem = new_mem;

    return (CMemChunk) { .mem = self->mem, .size = new_size };
}

void
c_allocator_default_free_impl(CAllocator* self, CMemChunk* mem)
{
    c_assert_debug(self, "");
    c_assert_debug(self->mem, "");

    free(self->mem);
    self->mem = null;
    mem->mem = null;
    mem->size = 0;
}

CMemChunk
c_allocator_default_alloc_aligned_impl(CAllocator* self, usize alignment, usize size)
{
    c_assert_debug(self, "");
    c_assert(!self->mem, "Can not allocate on an allocated memory");

    self->mem = c_allocator_default_aligned_alloc(alignment, size);
    c_assert(self->mem, ERROR_MEM_ALLOC);

    return (CMemChunk) { .mem = self->mem, .size = size };
}

CMemChunk
c_allocator_default_realloc_aligned_impl(CAllocator* self, CMemChunk old_mem, usize alignment, usize new_size)
{
    c_assert_debug(self, "");
    c_assert_debug(self->mem, "");

    void* new_mem = c_allocator_default_aligned_realloc(old_mem.mem, alignment, new_size);
    c_assert(new_mem, ERROR_MEM_ALLOC);
    self->mem = new_mem;

    return (CMemChunk) { .mem = self->mem, .size = new_size };
}

void
c_allocator_default_free_aligned_impl(CAllocator* self, CMemChunk* mem)
{
    c_assert_debug(self, "");
    c_assert_debug(self->mem, "");
    c_allocator_default_aligned_free(self->mem);
    self->mem = null;
}
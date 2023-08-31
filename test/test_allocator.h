#ifndef CLIB_TEST_ALLOCATOR_H
#define CLIB_TEST_ALLOCATOR_H

#include <clib/unittest.h>
#include <clib/allocator.h>
#include <clib/log.h>
#include <clib/list.h>

#include <string.h>

static inline void c_test_allocator(CUnit_Test* unit_test)
{
    c_test_register(unit_test, c_test_default_allocator);
    c_test_register(unit_test, c_test_arena_allocator);
}

void c_test_default_allocator(CUnit_Test* self)
{
    CAllocator allocator = c_allocator_get_default_allocator();

    CMemChunk arr = c_allocator_default_alloc(&allocator, sizeof(char) * 11);

    for(int iii = 0; iii < 10; ++iii)
    {
        ((char*)arr.mem)[iii] = 'a' + iii;
    }
    ((char*)arr.mem)[10] = '\0';

    const char* gt = "abcdefghij";
    TEST_CHECK(strcmp(gt, arr.mem) == 0);

    c_allocator_default_free(&allocator, &arr);
}

void c_test_arena_allocator(CUnit_Test* self)
{
    c_list(int) buf = c_list_new_with_capacity(int, 50);
    CAllocator arena_allocator = c_allocator_arena_init(buf, c_list_capacity(buf));

    CMemChunk mem1 = c_allocator_arena_alloc(&arena_allocator, 5);
    int x = mem1.valid;
    bool y = x;
    if(y) puts("-----------");
    TEST_REQUIRE(mem1.valid);

    CMemChunk mem2 = c_allocator_arena_alloc(&arena_allocator, 5);
    if(mem2.valid) puts("yummy");
    else printf("%ld\n", (usize)mem2.mem);
    int x2 = mem2.valid;
    printf("--- %d\n", x2);
    bool y2 = x2;
    if(y2) puts("-----------");
    TEST_REQUIRE(mem2.valid);

    CMemChunk mem3 = c_allocator_arena_alloc(&arena_allocator, 5);
    TEST_REQUIRE(!mem3.valid);

    c_list_free(buf);
}

#endif // CLIB_TEST_ALLOCATOR_H
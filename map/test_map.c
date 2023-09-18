#include <map.h>

#undef NDEBUG
#include <assert.h>
#include <stdio.h>

#define STR_W_LEN(str) str, sizeof(str)
#define INT_W_LEN(i) &(int){i}, sizeof(int)

int main(void) {
    Map* map = map_create(100, 11, sizeof(int));

    assert(map_insert(&map, STR_W_LEN("abc"), INT_W_LEN(1)));
    assert(map_insert(&map, STR_W_LEN("ahmed here"), INT_W_LEN(2)));
    assert(map_insert(&map, STR_W_LEN("abcd"), INT_W_LEN(3)));  // test collision
    assert(map_insert(&map, STR_W_LEN("abc"), INT_W_LEN(4)));   // test override

    assert(*(int*)map_get(map, STR_W_LEN("abcd")) == 3);
    assert(*(int*)map_get(map, STR_W_LEN("abc")) == 4);
    assert(map_get(map, STR_W_LEN("xyz")) == NULL);             // test not exist

    map_destroy(&map);
}

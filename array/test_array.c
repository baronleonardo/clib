#include <array.h>

#include <stdio.h>
#include <assert.h>

int main() {
    Array* array = array_create(sizeof(int));

    // test_push
    array_push(array, &(int){12});
    array_push(array, &(int){13});
    array_push(array, &(int){14});
    array_push(array, &(int){15});
    array_push(array, &(int){16});
    assert(array_get_len(array) == 5);

    // test array_pop
    int* data = array_pop(array);
    assert(*data == 16);

    // test array_remove_range
    int* removed_range = array_remove_range(array, 1, 3);
    assert(removed_range[0] == 13);
    assert(removed_range[1] == 14);
    assert(removed_range[2] == 15);
    assert(array_get_len(array) == 1);
    assert(((int*)array)[0] == 12);

    array_destroy(array);
}
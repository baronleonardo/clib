#include <array.h>

#include <stdio.h>
#undef NDEBUG
#include <assert.h>

int main(void) {
    Array array = array_create(sizeof(int));
    ArrayError error = 0;

    // test_push
    array = array_push(array, &(int){12}, &error);
    assert(error == 0);
    array = array_push(array, &(int){13}, &error);
    assert(error == 0);
    array = array_push(array, &(int){14}, &error);
    assert(error == 0);
    array = array_push(array, &(int){15}, &error);
    assert(error == 0);
    array = array_push(array, &(int){16}, &error);
    assert(error == 0);

    assert(array_get_len(array, &error) == 5);
    assert(error == 0);

    // test array_pop
    const int* data = array_pop(array, &error);
    assert(error == 0);
    assert(*data == 16);

    // test array_remove_range
    const int* removed_range = array_remove_range(array, 1, 3, &error);
    assert(error == 0);
    assert(removed_range[0] == 13);
    assert(removed_range[1] == 14);
    assert(removed_range[2] == 15);

    assert(array_get_len(array, &error) == 1);
    assert(error == 0);
    assert(((int*)array)[0] == 12);

    array = array_destroy(array, &error);
    assert(error == 0);

    return 0;
}

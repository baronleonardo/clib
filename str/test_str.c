#include <str.h>

#undef NDEBUG
#include <assert.h>
#include <string.h>

int main(void) {
    Str str = str_create("Ahmed is here", 100);

    assert(str_remove(&str, "here", 100));
    assert(strcmp(str.data, "Ahmed is ") == 0);

    str_add(&str, "here", 100);
    assert(strcmp(str.data, "Ahmed is here") == 0);

    str_destroy(&str);
}

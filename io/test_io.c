#include <io.h>

#undef NDEBUG
#include <assert.h>
#include <string.h>

#define STR(str) (str), sizeof((str))

int main(void) {
    enum { buf_len = 100 };
    char buf[buf_len];

    // test io_file_read
    File f = io_file_open(STR(test_assets "/file1.txt"), "r");
    size_t amount_read = io_file_read(f, buf, buf_len);
    assert(amount_read);
    buf[amount_read] = '\0';
    assert(strncmp(buf, "May peace be upon you\n", amount_read) == 0);
    io_file_close(&f);

    // test io_file_write
    f = io_file_open(STR(test_assets "/out-file.txt"), "w");
    const char* out = "This is an attemped to do the right thing\n";
    size_t out_len = strnlen(out, buf_len);
    assert(memcpy(buf, out, out_len));
    assert(io_file_write(f, buf, out_len));
    io_file_close(&f);
    io_delete(STR(test_assets "/out-file.txt"));
    io_delete(STR(test_assets "/folder"));
}

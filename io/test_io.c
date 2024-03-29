#include <io.h>

#undef NDEBUG
#include <assert.h>
#include <string.h>

#define STR(str) (str), (sizeof((str)) - 1)

static bool
handler(const char* path, size_t path_len, void* extra_data);

int main(void) {
    enum { buf_len = 100 };
    char buf[buf_len];

    // test io_file_read
    File f = io_file_open(STR(test_assets "/file1.txt"), "r");
    size_t amount_read = io_file_read(&f, buf, buf_len);
    assert(amount_read);
    buf[amount_read] = '\0';
    assert(strncmp(buf, "May peace be upon you\n", amount_read) == 0);
    io_file_close(&f);

    // test io_file_read - utf8
    f = io_file_open(STR(test_assets "/فايل2.txt"), "r");
    amount_read = io_file_read(&f, buf, buf_len);
    assert(amount_read);
    buf[amount_read] = '\0';
    assert(strncmp(buf, "الحمد لله\n", amount_read) == 0);
    io_file_close(&f);

    // test io_file_write
    f = io_file_open(STR(test_assets "/out-file.txt"), "w");
    const char* out = "بسم الله الرحمن الرحيم\n";
    size_t out_len = strlen(out);
    assert(memcpy(buf, out, out_len));
    assert(io_file_write(&f, buf, out_len));
    io_file_close(&f);
    io_delete(STR(test_assets "/out-file.txt"));

    // test io_delete_recursively
    io_dir_create(STR(test_assets "/folder"));
    File file1 = io_file_open(STR(test_assets "/folder/1.txt"), "w");
    io_file_close(&file1);
    io_dir_create(STR(test_assets "/folder/folder2"));
    File file2 = io_file_open(STR(test_assets "/folder/folder2/.2.txt"), "w");
    io_file_close(&file2);
    io_delete_recursively(STR(test_assets "/folder"));

    // test io_dir_empty
    io_dir_create(STR(test_assets "/folder"));
    assert(io_dir_empty(STR(test_assets "/folder")));
    io_delete(STR(test_assets "/folder"));

    // test io_foreach
    bool file_found = false;
    io_foreach(STR(test_assets), handler, &file_found);
    assert(&file_found);

    // test io_delete
    io_dir_create(STR(test_assets "/folder2"));
    assert(io_exists(STR(test_assets "/folder2")));
    io_delete(STR(test_assets "/folder2"));
}

bool
handler(const char* path, size_t path_len, void* extra_data) {
    (void)path_len;

    if(strstr(path, "فايل")) {
        *(bool*)extra_data = true;
    }

    return true;
}
